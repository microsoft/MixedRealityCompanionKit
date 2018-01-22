-- Copyright (c) Microsoft Corporation. All rights reserved.
-- Licensed under the MIT License. See LICENSE in the project root for license information.
-------------------------------------------------
-- Declare Needed Globals
-------------------------------------------------
global importAllFiles
global dontDecimateLessThan
global qualitySurfAreaCalc
-------------------------------------------------
--		SCRIPT FUNCTIONS
-------------------------------------------------
-- Helper function to turn string settings into a proper Max Array.
fn createInputArrays inStringStream turnToInts =
(
	tempInputArray = #()
	--Read stringStream and append each entry seperated by a comma as int or string depending on turnToInts
	if turnToInts == false then
	(
		while ((eof inStringStream) == false) do
		(
			local entry = readDelimitedString inStringStream ","
			append tempInputArray entry
		)
	) else (
		while ((eof inStringStream) == false) do
		(
			local intEntry = readDelimitedString inStringStream ","
			intEntry = intEntry as integer
			append tempInputArray intEntry
		)
	)
	close inStringStream
	return tempInputArray	
)

-- Function to help recursively look through folders for finding files
fn getFilesRecursive root pattern =
(
	dir_array = GetDirectories (root+"/*")
	for d in dir_array do
		join dir_array (GetDirectories (d+"/*"))
	--make sure to append back in root directory
	append dir_array root
    --make array for all files and loop through all directories			
	my_files = #()
	if importAllFiles == true then
	(
		for f in dir_array do
			join my_files (getFiles (f + pattern))
	) else (
			join my_files (getFiles (root + pattern))
	)
	my_files
)

-- Function to run through all the objects in the scene to calculate how objects will be decimated and to 
-- print out stats to external json file.
fn getStats property target obj boolArea out_file =
(
	undo off (
		format "\"%\" : {" property to:out_file
		format "\"targetplatform\" : \"%\",\n" target to:out_file
		-- stats array order= polycount, vertexcount, partcount, surfacearea
		stats = #(0,0,0,0.0)
		decimatedParts = #()
		-- Add a divide factor to help keep from int 32 overflow on large files
		divideFactor = 100.0
		setUserProp obj "LOD" property   
		for m in obj do
		(			
			setUserProp m "name" m.name
			if 	ClassOf m == Editable_Mesh then
			(
				theCount = meshop.getnumfaces m
				polycount = theCount
				setUserProp m "polycount" polycount  
				stats[1]  += polycount
				vertexcount = meshop.getnumverts m
				verexCountLog = vertexcount
				setUserProp m "vertexcount" vertexcount  
				stats[2]  += vertexcount
				stats[3] += 1
				--EXPENSIVE OPERATION: only calculated once then saved in user properties
				if boolArea == true then
				(
					resetxform m
					maxOps.CollapseNode m true
					surfacearea = 0.0
					--Use total face surface area -- THIS CAN BE SLOW, but will yeild better results. Turned off by default
					if qualitySurfAreaCalc == true then
					(
						for f = 1 to theCount do 
						(
							surfacearea += (meshop.getFaceArea m f / divideFactor)
						)
					--Use BB Volume instead of total face surface area -- THIS IS MUCH FASTER, but yeilds inaccurate results on building a proper ratio of parts area to total scene area.
					) else (
							local vMin = (m.min / divideFactor)
							local vMax = (m.max / divideFactor)

							local fX = vMax.x - vMin.x
							local fY = vMax.y - vMin.y
							local fZ = vMax.z - vMin.z
							surfacearea = fX * fY * fZ
							abs(surfacearea)
					)	
					setUserProp m "surfacearea" surfacearea 
					stats[4]  += surfacearea
							
				)
				--Get Decimated Parts
				if getUserProp m "decimated" != undefined then
				(
					decimatedParts += m
				)
			)
		)
		-- Code for formatting and writing stats to a JSON file.
		format "\"polycount\" : \"%\",\n" stats[1] to:out_file
		format "\"vertexcount\" : \"%\",\n" stats[2] to:out_file
		format "\"partcount\" : \"%\"" stats[3] to:out_file
		if boolArea == true then
		(
			format ",\n\"surfacearea\" : \"%\"" stats[4] to:out_file
		)
		if decimatedParts.count > 0 then
		(
			format ",\n\"decimatedPartsCount\" : \"%\"" decimatedParts.count to:out_file
			format ",\n\"decimatedParts\" : { \n" to:out_file
			for p = 1 to decimatedParts.count do
			(
				format "\"%\" : {" p to:out_file
				--format "\"%\" : \"%\", " p decimatedParts[p].name to:out_file
				format "\"Name\" : \"%\", " decimatedParts[p].name to:out_file
				local tempOrigVerts = getUserProp decimatedParts[p] "decimated_origVerts"
				format "\"originalVerts\" : \"%\", " tempOrigVerts to:out_file
				local tempTargetVerts = (getUserProp decimatedParts[p] "decimated_targetVerts") as integer
				format "\"targetVerts\" : \"%\", " tempTargetVerts to:out_file
				local tempActualVerts = getUserProp decimatedParts[p] "vertexcount"
				format "\"endResultVerts\" : \"%\" " tempActualVerts to:out_file
				if p < decimatedParts.count then
				(
					format "}, \n" to:out_file
				) else (
					format "} \n" to:out_file
				)
			)
			format "\n} \n" to:out_file
		)
		format "\n}, \n" to:out_file
		return stats
	)
)

-- Function to create a new LOD based off of original mesh
fn createLOD level toClone =
(
	outNodes = #()
	maxOps.CloneNodes toClone expandHierarchy:on cloneType:#copy actualNodeList:&srcNodes newNodes:&outNodes
	for o in (outNodes as array) do 
	(
		--rename files to LOD label
		oldName = o.name
		SelectionCount = 9
		startInt = oldName.count - SelectionCount
		NewName = replace o.name (startInt+1) SelectionCount level
		o.name = NewName	
		setUserProp o "name" o.name
		setUserProp o "LOD" level
		--quick weld all verts @ small threshold then smooth
		if classOf o == Editable_Mesh then
		(
			undo off(
				with redraw off
				(
					if meshop.getnumverts o < 2 then
					(
						--Delete any objects with no faces.
						local indexo = FindItem outNodes o
						if indexo != 0 then
						(
							deleteItem outNodes indexo
						)
						delete o
					)
					
					meshOp.weldVertsByThreshold o o.verts 0.00001

				)
			)
		)
	)
	return outNodes
)

-- Route to decimate using the Pro Optimizer Mod. This usually produces better results for objects that do not have UVs
fn decimateProOpt obj desiredVertsWithWeightPerc =
(
	max modify mode 
	max select none
	select obj
	modPanel.addModToSelection (ProOptimizer ())  ui:on
	select obj
	$.modifiers[#ProOptimizer].LockMat = off
	$.modifiers[#ProOptimizer].OptimizationMode = 1
	$.modifiers[#ProOptimizer].KeepNormals = on
	$.modifiers[#ProOptimizer].NormalMode = 1
	$.modifiers[#ProOptimizer].MergePoints = off
	$.modifiers[#ProOptimizer].MergePointsThreshold = 0.0
	$.modifiers[#ProOptimizer].Calculate = on
	$.modifiers[#ProOptimizer].VertexPercent = desiredVertsWithWeightPerc

	addModifier  $  (smooth autosmooth:true threshold:35)

	collapseStack obj
)

-- Route to decimate using the Pro Optimizer Mod. This usually produces better results for objects that have UVs
fn decimateMultiRes obj desiredVertsWithWeightPerc =
(
    max select none
	select obj --have to select the object to generate multires
	max modify mode  --switch mod panel to modify mode so can exicute generate button
	local decimateMod = MultiRes BoundaryMetric:true baseVertices:false mergeThreshold:0.025 mergeWithinMesh:true multiVertNorm:false vertexPercent:desiredVertsWithWeightPerc
	addModifier obj decimateMod
	 obj.modifiers[#MultiRes].reqGenerate = true
	addModifier  obj  (Edit_Normals())
	addModifier  obj  (smooth autosmooth:true threshold:35)
	select obj
	collapseStack obj
)

-- Main Function to calculate how much to decimate each part and which decimation method to use
fn decimateLOD objArray surfArea parts targetVerts dontDecimateLessThan =
(
	decimateOBJs = #()
	decimateSurfArea = 0.0
	surfArea = surfArea
	exrtaVerts = 0
	decVerts = 0
	--Run A first pass to collect objects to be decimated and get a wieghted vertex count of parts with left over polys
	for o in objArray do
	(
		if classOf o == Editable_Mesh then
		(  
			partarea = (getUserProp o "surfacearea")
			partVerts = meshop.getnumverts o
			sizeRatio = partarea / surfArea
			sizeRatio = abs(sizeRatio as float)
			desiredVerts = (sizeRatio * targetVerts)
			if (desiredVerts < partVerts and partVerts > dontDecimateLessThan) then
			(
				decimateSurfArea += partArea
				decimateOBJs += o 
			) else (
				decVerts += partVerts
			)
		)
	)
	
	print "Crunching Now"  
	windows.processPostedMessages()
	
	-- Now that we have run a first pass, go thrpough and actually decimate the parts that will need decimation
	for o in decimateOBJs do
	(
	undo off (
			targetVertWeight = decVerts
			targetWithWeight = abs(targetVertWeight - targetVerts)
			partarea = (getUserProp o "surfacearea")
			partVerts = meshop.getnumverts o 
			sizeRatio = partarea / decimateSurfArea
			sizeRatio = abs(sizeRatio as float)
			desiredVertsWithWeight = sizeRatio * targetWithWeight
			--convert to a percentage
			desiredVertsWithWeightPerc = ( desiredVertsWithWeight / partVerts )
			desiredVertsWithWeightPerc = ( desiredVertsWithWeightPerc * 100 )
			
			if ( desiredVertsWithWeight > partVerts ) then
			(
				decVerts += partVerts
			) else (
			
				select o
				 --switch mod panel to modify mode so can execute generate button in UI
				max modify mode
				-- Decide which decimation method to use based on if it has a material or not
				if classof o.material == Multimaterial then 
				(
					decimateMultiRes o desiredVertsWithWeightPerc
				) else (
					try
					(
						if o.material.DiffuseMap != undefined then (
						   decimateMultiRes o desiredVertsWithWeightPerc
						) else (
							decimateProOpt o desiredVertsWithWeightPerc
						)
					
					) catch (
						decimateProOpt o desiredVertsWithWeightPerc
					)
				)
				 
				setUserProp o "decimated" "true"
				setUserProp o "decimated_origVerts" partVerts
				setUserProp o "decimated_targetVerts" desiredVertsWithWeight
				
				decVerts += desiredVertsWithWeight
			)
			decimateSurfArea -= partArea
	)
	)
	--switch back to create panel for speed
	max create mode 
)

-- WARNING: Still experimenting with this functiuon. The goal is to combine parts with same materials to save on draw calls.
-- This can end up being slower depending on usecase as you cant cull out parts not int he render frame
fn attachPartsWithSameMaterials objArray =
(
	mergedSurfArea = 0.0 
	for i in (objArray as array) where not isdeleted i do
	(
		convertToPoly i

		if classof i == Editable_Poly then
		(
			for m in (objArray as array) where superclassof m == GeometryClass do
			(
				if m.material == i.material and m != i then
				(
					mergedSurfArea =  (getUserProp m "surfacearea") +  (getUserProp i "surfacearea")
					setUserProp i "surfacearea" mergedSurfArea  
					i.attach m i
					local removeEntry = findItem objArray m
					if removeEntry != 0 then 
					(
						deleteItem objArray removeEntry
					)
				)
			)
		)
		convertToMesh i
	)
	return objArray
)

-------------------------------------------------
--		MAIN SCRIPT EXECUTION
-------------------------------------------------
dir = inputDir
targetPlatsStream = inTargetPlats as stringStream
targetPlats = createInputArrays targetPlatsStream false
targetVertsCountStream = inTargetVertCount as stringStream
targetVertsCount = createInputArrays targetVertsCountStream true
dontDecimateLessThan = inDontDecimateLessThan as integer
combinePartsWithSameMaterial = inCombinePartsWithSameMaterial as booleanClass
qualitySurfAreaCalc = inQualitySurfAreaCalc as booleanClass
importAllFiles = inImportAllFiles as booleanClass

--Keep an array of created LODS
createdLODS = #()

if dir != undefined then
(
	--Create Directory we will export to and create a json file to output data
	makeDir (dir + @"\Export")
	out_name = (dir +@"\Export" + "\mesh.json")
	out_file = createfile out_name
	format "{\n" to:out_file
	format "\"DateTimeStarted\" : \"%\",\n" localTime to:out_file
	
	-- Check if there is an overwrite settings file, if so load those values instead
	--WARNING: If the zip contains more than one settings.ini we only take the first one
	overWriteSettingsINI = #()
	overWriteSettingsINI = getFilesRecursive dir @"\settings.ini"
	if overWriteSettingsINI.count > 0 then
	(
		format "\"CustomSettings\" : \"true\",\n" to:out_file
		SettingsINI = overWriteSettingsINI[1]
		if doesFileExist SettingsINI then
		(
			if (getINISetting SettingsINI "Decimation" "targetPlatforms") != undefined then
			(
				targetPlatsTemp = (getINISetting SettingsINI "Decimation" "targetPlatforms") as stringStream
				targetPlats = createInputArrays targetPlatsTemp false
			)
			if (getINISetting SettingsINI "Decimation" "targetVertsCount") != undefined then
			(
				targetVertsCountTemp = (getINISetting SettingsINI "Decimation" "targetVertsCount") as stringStream
				targetVertsCount = createInputArrays targetVertsCountTemp true
			)
			if (getINISetting SettingsINI "Decimation" "dontDecimateLessThan") != undefined then
			(
				dontDecimateLessThan = (getINISetting SettingsINI "Decimation" "dontDecimateLessThan") as integer
			)
			if (getINISetting SettingsINI "Decimation" "CombineSameMats") != undefined then
			(
				inCombinePartsWithSameMaterial = (getINISetting SettingsINI "Decimation" "CombineSameMats") as booleanClass
			)
			if (getINISetting SettingsINI "Decimation" "qualitySurfAreaCalc") != undefined then
			(
				qualitySurfAreaCalc = (getINISetting SettingsINI "Decimation" "qualitySurfAreaCalc") as booleanClass
			)
			if (getINISetting SettingsINI "Decimation" "allFiles") != undefined then
			(		
				importAllFiles = (getINISetting SettingsINI "Decimation" "allFiles") as booleanClass
			) 
		)
	) else (
			format "\"CustomSettings\" : \"false\",\n" to:out_file
	)
	roots = #()

	filesToBatch = #()
	--supported file types
	filesToBatch += getFilesRecursive dir @"\*.catproduct"
	filesToBatch += getFilesRecursive dir @"\*.jt"
	filesToBatch += getFilesRecursive dir @"\*.stp"
	filesToBatch += getFilesRecursive dir @"\*.step"
	filesToBatch += getFilesRecursive dir @"\*.sldasm" -- solidworks 
	filesToBatch += getFilesRecursive dir @"\*.sldprt" -- solidworks 
	filesToBatch += getFilesRecursive dir @"\*.fbx"
	filesToBatch += getFilesRecursive dir @"\*.obj"
	filesToBatch += getFilesRecursive dir @"\*.dwg"
	filesToBatch += getFilesRecursive dir @"\*.dxf"
	filesToBatch += getFilesRecursive dir @"\*.ige"
	filesToBatch += getFilesRecursive dir @"\*.iges"
	filesToBatch += getFilesRecursive dir @"\*.igs"

	FbxImporterSetParam "Animation" false
	FbxImporterSetParam "Skin" false
	FbxImporterSetParam "Shape" false
	FbxImporterSetParam "SmoothingGroups" true

	---import all meshes in dir and clean up linking
	for f = 1 to filesToBatch.count do
	(
		importFile (filesToBatch[f] as string) #noPrompt
		fileExt = getFilenameType filesToBatch[f]
		--If an Autocad File we need to do some special processing
		--WARNING: For Autocade we only allow one file to be in the zip.
		if fileExt == ".dxf" or fileExt == ".dwg" then 
		(
			undo off (
				print "AutoCad File" 
				windows.processPostedMessages()
				select objects
				-- Assign a grey material to all objects so they dont come in as flat black
				newmat = StandardMaterial()
				newmat.name = "flatGrey"
				for i in geometry do 
				(
					i.material = newmat
					addModifier  i  (smooth autosmooth:true threshold:35)
					collapseStack i
				)
				--Setup a clean transform for all the mesh objects
				objs = for i in selection collect i
				for i in objs do
				(
					i.transform.controller = prs()
					if classof i.parent == LinkComposite do i.parent = undefined
				)
				--Delete any undeeded mesh
				for i in objs where (classof i == LinkComposite) do delete i
			)
		)
		 --give imported meshes unique name so multi files dont stomp on eachother
		for n in selection do n.name = ((getFilenameFile filesToBatch[f]) + "_" + n.name + "_Source")
		--kill all hidden objects
		for  o in (objects as array) where o.isHiddenInVpt do delete o
		--kill all dummys
		for i in (selection as array) where classof i == Dummy do delete i
		--unlink all objects
		for n in selection do n.parent = undefined
		--explode all groups
		for o in helpers where isGroupHead o do explodeGroup o
		--Remove any splines/shapes
		for  o in (objects as array) where superclassof o == Shape do delete o
		-- JT files normally contain duplicate data of mesh and breps. We will delete the mesh and then turn breps into mesh.
		if fileExt == ".jt" then 
		(
			for m in (objects as array) where (findstring m.name "shape") != undefined do delete m
		)
		--Make sure everything is a EditableMesh
		for m in (objects as array) where superClassOf m == GeometryClass do convertToMesh m
		--Make a root node to link to mesh then parent imported mesh to node
		rootObj = point name:((getFilenameFile filesToBatch[f]) + "_Source")
		roots += rootObj
		for p in (geometry as array) where findstring p.name (getFilenameFile filesToBatch[f]) != undefined do p.parent = rootObj
	)
	-- garbage collect to help clear out memory
	gc()
	--link cleaned mesh to a root note
	rootSource = point name:"Source"
	for r in roots do r.parent = rootSource
	--output total meshes loaded in bundle
	format "\"ModelsInZip\" : \"%\",\n" filesToBatch.count to:out_file
	--Json output target LOD Levels
	format "\"TargetLODs\" : {" filesToBatch.count to:out_file
	for lod = 1 to targetPlats.count do
	(
		format "\"LOD%\" : {" lod to:out_file
		format "\"TargetPlatform\" : \"%\"," targetPlats[lod] to:out_file
		format "\"TargetPlatformVertexCount\" : \"%\"" targetVertsCount[lod] to:out_file
		if lod < targetPlats.count then
		(
			format "}," to:out_file
		) else (
			format "} \n" to:out_file
		)
	)
	format "\n}, \n" to:out_file
	format "\"DontDecimateVertsUnder\" : \"%\",\n" dontDecimateLessThan to:out_file
	--save off soucre objects then calculate stats
	sourceObjects = objects as array
	sourceStats = getStats "Source" "Source-NoEdits" sourceObjects true out_file
	if combinePartsWithSameMaterial == true then 
	(
		sourceObjects = attachPartsWithSameMaterials sourceObjects
	)
	--Render thumbnail of mesh
	hideByCategory.helpers = true
	select $*
	max zoomext sel all
	Skylight pos:[0,0,0] name:"tmpLight" multiplier:1
	renderFileName = (dir + @"\Export\" + "mesh.png")
	undo off (
		Render frame:#current renderType:#blowup outputwidth:256 outputheight:256 outputfile:renderFileName vfb:false
	)
	delete $tmpLight
	--Create a master root
	rootAll = point name:"Root"
	rootSource.parent = rootAll
	--Create first level LOD0
	LOD0 = createLOD "LOD0" rootSource
	append createdLODS "LOD0"
	LOD0Stats = getStats "LOD0" "Source-Welded" LOD0 false out_file
	gc()
	--Create LODS in target platforms, if source vert count is over targets
	for l = 1 to targetPlats.count do
	(
		if LOD0Stats[2] > targetVertsCount[l] then
		(
				lodString = "LOD" + (l as string)
				print "Creating LOD: " + lodString  
				windows.processPostedMessages()
				TempLOD = createLOD lodString rootSource
				append createdLODS lodString
				TempLODDec = decimateLOD TempLOD sourceStats[4] LOD0Stats[2] targetVertsCount[l] dontDecimateLessThan
				if combinePartsWithSameMaterial == true then 
				(
					mergedMats = attachPartsWithSameMaterials TempLOD
					TEMPLODStats = getStats lodString targetPlats[l] mergedMats  false out_file
				) else (
					TEMPLODStats = getStats lodString targetPlats[l] TempLOD  false out_file
				)
				gc()
		)
	)
	--Export FBXs Per LOD LEVEL
	for lodLevel = 1 to createdLODS.count do 
	(
		hide $*
		max select none
		selectCmd = "select $" + (createdLODS[lodLevel] as string) + "...*"
		execute selectCmd
		selection.isHidden = off
		FBXExporterSetParam "Animation" false
		FBXExporterSetParam "Skin" false
		FBXExporterSetParam "Shape" false
		FBXExporterSetParam "EmbedTextures" true
		FBXExporterSetParam "SmoothingGroups" true
		FBXExporterSetParam "SmoothMeshExport" false
		FBXExporterSetParam "Triangulate" false
		FBXExporterSetParam "UpAxis" "Y"
		exportFile (dir + @"\Export\" + "mesh_" + createdLODS[lodLevel] + ".fbx") #noPrompt selectedOnly:true 
		selection.isHidden = on
	)
	format "\"DateTimeEnded\" : \"%\"\n" localTime to:out_file
	format "}" to:out_file
	close out_file
)
quitMax #noPrompt