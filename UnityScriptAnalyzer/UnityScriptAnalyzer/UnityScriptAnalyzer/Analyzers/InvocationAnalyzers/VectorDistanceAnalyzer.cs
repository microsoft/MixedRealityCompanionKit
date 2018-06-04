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
    /// This analyzer searches for Unity's vector distance function calls
    /// The Distance() function involves a sqrt() operation which is significantly  more expensive than the multiplication and addition operations also involved
    /// Developers should consider using sqrMagnitude where applicable to avoid this extra sqrt() processing load
    /// </summary>
    [DiagnosticAnalyzer(LanguageNames.CSharp)]
    public class VectorDistanceAnalyzer : DiagnosticAnalyzer
    {
        private static readonly string Message = "Consider using sqrMagnitude if applicable instead of Distance() to eliminate expensive sqrt calculations";

        private static readonly string Title = "Vector Distance()";

        internal static DiagnosticDescriptor Rule
            => new DiagnosticDescriptor(DiagnosticIDs.VectorDistance, Title, Message, Consts.Category, DiagnosticSeverity.Warning, true);

        public override ImmutableArray<DiagnosticDescriptor> SupportedDiagnostics => ImmutableArray.Create(Rule);

        public override void Initialize(AnalysisContext context) => context.RegisterSyntaxNodeAction(Analyze, SyntaxKind.InvocationExpression);

        private static void Analyze(SyntaxNodeAnalysisContext context)
        {
            var node = context.Node as InvocationExpressionSyntax;

            var methodSymbol = context.SemanticModel.GetSymbolInfo(node).Symbol as IMethodSymbol;
            if (methodSymbol == null)
                return;

            string methodName = methodSymbol.ToString();

            // If this is the Unity's distance function for either Vector2 or Vector3 classes, then create diagnostic 
            if (methodName.StartsWith("UnityEngine.Vector3.Distance") || methodName.StartsWith("UnityEngine.Vector2.Distance"))
            {
                Helpers.CreateAndReportDiagnostic(Rule, node.GetLocation(), ref context);
            }
        }
    }
}
