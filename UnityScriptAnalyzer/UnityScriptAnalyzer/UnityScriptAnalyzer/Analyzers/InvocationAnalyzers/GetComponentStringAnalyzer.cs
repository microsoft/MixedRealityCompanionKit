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
    /// This analyzer searches for invocations of Unity's GetComponent function using a string parameter.
    /// GetComponent and it's variation can be expensive to use. The GetComponent(string) version though is particularly costly
    /// Multiple times more so than it's counterpart parameter versions
    /// </summary>
    [DiagnosticAnalyzer(LanguageNames.CSharp)]
    public class GetComponentStringAnalyzer : DiagnosticAnalyzer
    {
        private static readonly string Message = "Avoid using GetComponent(String). Use the much less expensive GetComponent<T>() version";

        private static readonly string Title = "GetComponent(String)";

        internal static DiagnosticDescriptor Rule
            => new DiagnosticDescriptor(DiagnosticIDs.GetComponentString, Title, Message, Consts.Category, DiagnosticSeverity.Warning, true);

        public override ImmutableArray<DiagnosticDescriptor> SupportedDiagnostics => ImmutableArray.Create(Rule);

        public override void Initialize(AnalysisContext context) => context.RegisterSyntaxNodeAction(Analyze, SyntaxKind.InvocationExpression);

        private static void Analyze(SyntaxNodeAnalysisContext context)
        {
            var node = context.Node as InvocationExpressionSyntax;

            var methodSymbol = context.SemanticModel.GetSymbolInfo(node).Symbol as IMethodSymbol;
            if (methodSymbol == null)
                return;

            string methodName = methodSymbol.ToString();

            if (methodName.Equals("UnityEngine.Component.GetComponent(string)"))
            {
                Helpers.CreateAndReportDiagnostic(Rule, node.GetLocation(), ref context);
            }
        }
    }
}
