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
    /// This analyzer searches for function calls setting or getting shader properties AND/OR AnimatorProperties via string instead of integer key
    /// Lookup via string is slower than via integer ID. If this operation is done repeatedly, it can incur a performance hit
    /// </summary>
    [DiagnosticAnalyzer(LanguageNames.CSharp)]
    public class UnityPropertyIDAnalyzer : DiagnosticAnalyzer
    {
        private const DiagnosticSeverity Severity = DiagnosticSeverity.Warning;

        private static readonly string ShaderMessage = "If used repeatedly, consider calling \"{0}\" with an integer key obtained via Shader.PropertyToId({1}), instead of using the string key directly";

        private static readonly string ShaderTitle = "Use Shader.PropertyToId instead of string key";

        private static DiagnosticDescriptor ShaderPropertyIDRule = new DiagnosticDescriptor(DiagnosticIDs.ShaderPropertyID,
                                                                    ShaderTitle,
                                                                    ShaderMessage,
                                                                    Consts.Category,
                                                                    DiagnosticSeverity.Warning,
                                                                    isEnabledByDefault: true,
                                                                    description: string.Empty);

        private static readonly string AnimatorMessage = "If used repeatedly, consider calling \"{0}\" with an integer key obtained via Animator.StringToHash({1}), instead of using the string key directly";

        private static readonly string AnimatorTitle = "Use Animator.StringToHash instead of string key";

        private static DiagnosticDescriptor AnimatorPropertyIDRule = new DiagnosticDescriptor(DiagnosticIDs.AnimatorPropertyID,
                                                                    AnimatorTitle,
                                                                    AnimatorMessage,
                                                                    Consts.Category,
                                                                    DiagnosticSeverity.Warning,
                                                                    isEnabledByDefault: true,
                                                                    description: string.Empty);
        public override ImmutableArray<DiagnosticDescriptor> SupportedDiagnostics => ImmutableArray.Create(ShaderPropertyIDRule, AnimatorPropertyIDRule);

        public override void Initialize(AnalysisContext context)
        {
            context.RegisterCompilationStartAction((CompilationStartAnalysisContext c) =>
            {
                c.RegisterSyntaxNodeAction(Analyze, SyntaxKind.InvocationExpression);
            });
        }

        private static void Analyze(SyntaxNodeAnalysisContext context)
        {
            var node = context.Node as InvocationExpressionSyntax;

            var methodSymbol = context.SemanticModel.GetSymbolInfo(node).Symbol as IMethodSymbol;
            if (methodSymbol == null)
                return;

            string methodName = methodSymbol.ToString();

            // We only care about Unity Shader & Animator property functions that take in string parameters
            // If this invocation is one of those methods and supplies a string parameter, then create a diagnostic. 
            // The string parameter check is done in the method symbol string comparison
            if (Consts.ShaderPropertyIDFunctions.Any(s => methodName.StartsWith(s)))
            {
                var args = node.ArgumentList.Arguments;
                string parameterKey = string.Empty;
                if (args != null && args.Count > 0)
                {
                    parameterKey = args[0].ToString();
                }

                Helpers.CreateAndReportDiagnostic(ShaderPropertyIDRule, node.GetLocation(), ref context, methodName, parameterKey);
            }
            else if (Consts.AnimatorPropertyIDFucntions.Any(s => methodName.StartsWith(s)))
            {
                var args = node.ArgumentList.Arguments;
                string parameterKey = string.Empty;
                if (args != null && args.Count > 0)
                {
                    parameterKey = args[0].ToString();
                }

                Helpers.CreateAndReportDiagnostic(AnimatorPropertyIDRule, node.GetLocation(), ref context, methodName, parameterKey);
            }
        }
    }
}