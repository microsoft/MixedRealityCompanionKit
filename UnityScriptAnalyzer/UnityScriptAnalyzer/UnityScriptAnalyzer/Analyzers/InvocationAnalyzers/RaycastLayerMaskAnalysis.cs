// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Microsoft.CodeAnalysis.Diagnostics;
using System.Collections.Immutable;

namespace UnityScriptAnalyzer.Analyzers.InvocationAnalyzers
{
    /// <summary>
    /// This analyzer checks for UnityEngine Physics.Raycast calls and if the layermask parameter was provided
    /// If using the Physics raycast calls in Unity, one should try to apply layermask parameters to reduce
    /// the number of operations that need to be performed, increasing performance
    /// </summary>
    [DiagnosticAnalyzer(LanguageNames.CSharp)]
    public class RaycastLayerMaskAnalysis : DiagnosticAnalyzer
    {
        private static readonly string Message = "Using function form \"{0}\", consider adding layerMask argument to optimize raycast performance";

        private static readonly string Title = "Add LayerMask to Physics.Raycast";

        internal static DiagnosticDescriptor Rule
            => new DiagnosticDescriptor(DiagnosticIDs.RaycastLayerMask, Title, Message, Consts.Category, DiagnosticSeverity.Warning, true);

        public override ImmutableArray<DiagnosticDescriptor> SupportedDiagnostics => ImmutableArray.Create(Rule);

        public override void Initialize(AnalysisContext context) => context.RegisterSyntaxNodeAction(Analyze, SyntaxKind.InvocationExpression);

        private static void Analyze(SyntaxNodeAnalysisContext context)
        {
            var node = context.Node as InvocationExpressionSyntax;

            var methodSymbol = context.SemanticModel.GetSymbolInfo(node).Symbol as IMethodSymbol;
            if (methodSymbol == null)
                return;

            string methodName = methodSymbol.ToString();

            // We only care about Raycast method invocations
            if(methodName.StartsWith("UnityEngine.Physics.Raycast") || methodName.StartsWith("UnityEngine.Physics.RaycastAll"))
            {
                // Loop through parameters for this method overload to see if layerMask argument is being used
                foreach (IParameterSymbol parameter in methodSymbol.Parameters)
                {
                    // If developer is using a layermask, then end analysis
                    if (parameter.Name.Equals("layerMask"))
                    {
                        return;
                    }
                }

                // If we reached this point, then we never found the layerMask parameter. Create diagnostic
                Helpers.CreateAndReportDiagnostic(Rule, node.GetLocation(), ref context, methodSymbol.ToString());
            }
        }
    }
}
