// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Microsoft.CodeAnalysis.Diagnostics;
using System.Collections.Immutable;
using System.Linq;

namespace UnityScriptAnalyzer.Analyzers.EmptyUnityCallbacks
{
    /// <summary>
    /// USA0001 - Empty Unity Callback Implementations
    /// This analyzer identifies Unity callback functions (ex: Update()) in Monobehavior script classes that have empty method bodies
    /// Even though the function overrides in a script class may have no operations (i.e empty), the process for Unity to call each method across
    /// the native/managed boundary is not cheap and wasted computation if it does nothing
    /// </summary>
    [DiagnosticAnalyzer(LanguageNames.CSharp)]
    public class EmptyUnityCallbackAnalyzer : DiagnosticAnalyzer
    {
        private static readonly string Message = "Unity callback function \"{0}\" is empty resulting in wasted overhead during runtime";

        private static readonly string Title = "Empty Unity Callback";

        internal static DiagnosticDescriptor Rule
            => new DiagnosticDescriptor(DiagnosticIDs.EmptyUnityCallback, Title, Message, Consts.Category, DiagnosticSeverity.Warning, true);

        public override ImmutableArray<DiagnosticDescriptor> SupportedDiagnostics => ImmutableArray.Create(Rule);

        public override void Initialize(AnalysisContext context) => context.RegisterSyntaxNodeAction(Analyze, SyntaxKind.MethodDeclaration);
        
        private static void Analyze(SyntaxNodeAnalysisContext context)
        {
            var methodSyntax = context.Node as MethodDeclarationSyntax;

            // Ensure we are only processing Method Declarations
            if (methodSyntax == null)
                return;

            var method = (IMethodSymbol)context.ContainingSymbol;
            var baseClass = method.ContainingType.BaseType;

            // Validate that the method name for this function matches one from Monobehavior unity
            // Validate that containing type of this function is not a base class 
            // and that IT'S base class is of Type UnityEngine MonoBehavior
            if (!Consts.UnityCallbacksFunctions.Contains(method.Name)
                || !(baseClass != null
                && baseClass.ContainingNamespace.Name.Equals("UnityEngine") 
                && baseClass.Name.Equals("MonoBehaviour")))
            {
                return;
            }

            // Now that we have confirmed we have syntax component for a method override we care about
            // Validate that the functions body is empty and if so, report the issue
            if (methodSyntax.Body != null && methodSyntax.Body.Statements.Count == 0 )
            {
                Helpers.CreateAndReportDiagnostic(Rule, methodSyntax.GetLocation(), ref context, method.Name);
            }
        }
    }
}
