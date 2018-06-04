// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Microsoft.CodeAnalysis.Diagnostics;
using System.Collections.Immutable;
using System.Linq;

namespace UnityScriptAnalyzer.Analyzers.InvocationAnalyzers
{

    /// <summary>
    /// This analyzer simply searches for calls of Unity's SendMessage & BroadcastMessage functions which are prohibitively expensive
    /// </summary>
    [DiagnosticAnalyzer(LanguageNames.CSharp)]
    public class UnityMessagingFuncAnalyzer : DiagnosticAnalyzer
    {
        private const DiagnosticSeverity Severity = DiagnosticSeverity.Warning;

        private static readonly string Message = "Unity SendMessage & BroadcastMessage functions are extremely expensive. Replace anywhere used with direct calls via casting if needed";

        private static readonly string Title = "Unity Messaging Functions";

        internal static DiagnosticDescriptor Rule
            => new DiagnosticDescriptor(DiagnosticIDs.UnityMessaging, Title, Message, Consts.Category, Severity, true);

        public override ImmutableArray<DiagnosticDescriptor> SupportedDiagnostics => ImmutableArray.Create(Rule);

        public override void Initialize(AnalysisContext context) => context.RegisterSyntaxNodeAction(Analyze, SyntaxKind.InvocationExpression);

        private static string[] messagingFuncs = {
                "UnityEngine.Component.SendMessage",
                "UnityEngine.GameObject.SendMessage",
                "UnityEngine.Component.BroadcastMessage",
                "UnityEngine.GameObject.BroadcastMessage" };

        private static void Analyze(SyntaxNodeAnalysisContext context)
        {
            var node = context.Node as InvocationExpressionSyntax;

            var methodSymbol = context.SemanticModel.GetSymbolInfo(node).Symbol as IMethodSymbol;
            if (methodSymbol == null)
                return;

            string methodName = methodSymbol.ToString();

            // If this invocation expression is a call to one of Unity's messaging functions, then create diagnostic
            if (messagingFuncs.Any(s => methodName.StartsWith(s)))
            {
                Helpers.CreateAndReportDiagnostic(Rule, node.GetLocation(), ref context);
            }
        }
    }
}
