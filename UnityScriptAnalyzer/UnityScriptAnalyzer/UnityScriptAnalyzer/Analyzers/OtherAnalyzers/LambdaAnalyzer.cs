// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Microsoft.CodeAnalysis.Diagnostics;
using System.Collections.Immutable;

namespace UnityScriptAnalyzer.Analyzers
{
    /// <summary>
    /// This analyzer searches for any lambda operations as they can be very costly to perform and should be avoided
    /// </summary>
    [DiagnosticAnalyzer(LanguageNames.CSharp)]
    public class LambdaAnalyzer : DiagnosticAnalyzer
    {
        private static readonly string Message = "Avoid using Lambdas as they cause additional Garbage Collection allocations";

        private static readonly string Title = "Lambda Usage";

        internal static DiagnosticDescriptor Rule
            => new DiagnosticDescriptor(DiagnosticIDs.LambdaUsage, Title, Message, Consts.Category, DiagnosticSeverity.Warning, true);

        public override ImmutableArray<DiagnosticDescriptor> SupportedDiagnostics => ImmutableArray.Create(Rule);

        public override void Initialize(AnalysisContext context) => context.RegisterSyntaxNodeAction(Analyze, new[] { SyntaxKind.ParenthesizedLambdaExpression, SyntaxKind.SimpleLambdaExpression });

        private static void Analyze(SyntaxNodeAnalysisContext context)
        {
            var lambda = context.Node as LambdaExpressionSyntax;

            // Any lambda expressions found should have a diagnostic created
            if (lambda != null)
            {
                Helpers.CreateAndReportDiagnostic(Rule, lambda.GetLocation(), ref context);
            }
        }
    }
}
