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
    /// LINQ Operations, although easy to write and helpful, can be very expensive and not efficient. 
    /// They should generally be avoided for performance reasons
    /// </summary>
    [DiagnosticAnalyzer(LanguageNames.CSharp)]
    public class LINQAnalyzer : DiagnosticAnalyzer
    {
        private static readonly string Message = "Avoid using LINQ which result in heavy allocations and poor performance";

        private static readonly string Title = "System.LINQ Usage";

        internal static DiagnosticDescriptor Rule
            => new DiagnosticDescriptor(DiagnosticIDs.LINQUsage, Title, Message, Consts.Category, DiagnosticSeverity.Warning, true);

        public override ImmutableArray<DiagnosticDescriptor> SupportedDiagnostics => ImmutableArray.Create(Rule);

        public override void Initialize(AnalysisContext context)
        {
            context.RegisterSyntaxNodeAction(Analyze, SyntaxKind.InvocationExpression);
            context.RegisterSyntaxNodeAction(AnalyzeQuery, SyntaxKind.QueryExpression);
        }

        private static void Analyze(SyntaxNodeAnalysisContext context)
        {
            var node = context.Node as InvocationExpressionSyntax;

            var methodSymbol = context.SemanticModel.GetSymbolInfo(node).Symbol as IMethodSymbol;
            if (methodSymbol == null)
                return;

            string methodNamespace = methodSymbol.ContainingNamespace.ToString();

            // If we are calling a method of the System.Linq namespace then we are using LINQ and create diagnostic
            if (methodNamespace.Equals("System.Linq"))
            {
                Helpers.CreateAndReportDiagnostic(Rule, node.GetLocation(), ref context);
            }
        }

        private static void AnalyzeQuery(SyntaxNodeAnalysisContext context)
        {
            // Any query expression is very expensive
            Helpers.CreateAndReportDiagnostic(Rule, context.Node.GetLocation(), ref context);
        }
    }
}
