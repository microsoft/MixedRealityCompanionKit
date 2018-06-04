// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Microsoft.CodeAnalysis.Diagnostics;
using System.Collections.Immutable;
using System.Linq;

namespace UnityScriptAnalyzer.Analyzers.RepeatingUnityCallbacks
{
    /// <summary>
    /// This analyzer looks for repeating Unity callback function bodies and then analyze the code content in those bodies for two rules
    /// 1) If the developer makes use of known expensive Unity operations (ex: GetComponent) in these code bodies, that is a performance issue of interest since it occurs every frame
    /// 2) If the developer makes use of accessing Camera.main every frame, this can be optimized by caching a reference to the camera to eliminate the lookup overhead
    /// </summary>
    [DiagnosticAnalyzer(LanguageNames.CSharp)]
    public class RepeatingUnityCallbackAnalyzers : DiagnosticAnalyzer
    {
        private static readonly string ExpensiveFunctionMessage = "Use of expensive function {0} has high overhead in repeating Unity callback function. Consider caching reference or perform operation loosely";

        private static readonly string ExpensiveFunctionTitle = "Expensive Update Function";

        public static DiagnosticDescriptor ExpensiveUpdateFunctionRule = new DiagnosticDescriptor(DiagnosticIDs.ExpensiveRepeatedUnityFuncs,
                                                                    ExpensiveFunctionTitle,
                                                                    ExpensiveFunctionMessage,
                                                                    Consts.Category,
                                                                    DiagnosticSeverity.Warning,
                                                                    isEnabledByDefault: true,
                                                                    description: "Operations in key repeating Unity callbacks should be scrutinized");

        private static readonly string CameraMainMessage = "Use of Camera.main actually involves a lookup for MainCamera every access. Consider cache reference instead";

        private static readonly string CameraMainTitle = "Cache Camera.Main Reference";

        public static DiagnosticDescriptor CacheMainCameraReferenceRule = new DiagnosticDescriptor(DiagnosticIDs.CacheMainCamera,
                                                                            CameraMainTitle,
                                                                            CameraMainMessage,
                                                                            Consts.Category,
                                                                            DiagnosticSeverity.Warning,
                                                                            isEnabledByDefault: true);

        public override ImmutableArray<DiagnosticDescriptor> SupportedDiagnostics => ImmutableArray.Create(ExpensiveUpdateFunctionRule, CacheMainCameraReferenceRule);

        public override void Initialize(AnalysisContext context)
        {
            context.RegisterCodeBlockStartAction<SyntaxKind>(AnalyzeUpdateCodeBlocks);
        }

        private static void AnalyzeUpdateCodeBlocks(CodeBlockStartAnalysisContext<SyntaxKind> context)
        {
            // If the current code block is not of a method, then stop analysis
            if (context.OwningSymbol.Kind != SymbolKind.Method)
            {
                return;
            }

            // If the method block does not return void and is not one of the repeating unity callback functions we care about (ex: Update()), then stop analysis
            var method = (IMethodSymbol)context.OwningSymbol;
            if (!method.ReturnsVoid || !Consts.RepeatingUnityCallbacksFunctions.Contains(method.Name))
            {
                return;
            }

            // Launch our sub analyzers to search for method invocations and member access expressions
            context.RegisterSyntaxNodeAction(IdentifyExpensiveFuncsAnalysis, SyntaxKind.InvocationExpression);
            context.RegisterSyntaxNodeAction(IdentifyExpensiveAccessAnalysis, SyntaxKind.SimpleMemberAccessExpression);
        }

        private static void IdentifyExpensiveFuncsAnalysis(SyntaxNodeAnalysisContext context)
        {
            var node = context.Node as InvocationExpressionSyntax;

            var methodSymbol = context.SemanticModel.GetSymbolInfo(node).Symbol as IMethodSymbol;

            if (methodSymbol == null)
                return;

            var method = methodSymbol.ToString();

            // If we find an expensive unity function being used in a repeating unity callback function, this is an area of code that should be investigated for performance improvements
            if (Consts.ExpensiveUnityFunctions.Any(s => method.StartsWith(s)))
            {
                Helpers.CreateAndReportDiagnostic(ExpensiveUpdateFunctionRule, node.GetLocation(), ref context, method);
            }
        }

        private static void IdentifyExpensiveAccessAnalysis(SyntaxNodeAnalysisContext context)
        {
            var identifer = (MemberAccessExpressionSyntax)context.Node;

            // The only identifier we care about is Camera.main. Do quick string comparison to avoid symbol lookup
            if (identifer == null || !identifer.Name.ToString().Equals("main"))
            {
                return;
            }

            var memberSymbol = context.SemanticModel.GetSymbolInfo(identifer.Name).Symbol;

            if (memberSymbol == null)
                return;

            string name = memberSymbol.ToString();

            // The developer is calling Camera.main instead of caching a reference, create a diagnostic
            if (name.StartsWith("UnityEngine.Camera.main"))
            {
                Helpers.CreateAndReportDiagnostic(CacheMainCameraReferenceRule, identifer.GetLocation(), ref context);
            }
        }
    }
}
