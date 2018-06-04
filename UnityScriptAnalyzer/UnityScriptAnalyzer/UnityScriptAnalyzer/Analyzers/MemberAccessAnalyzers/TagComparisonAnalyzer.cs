// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Microsoft.CodeAnalysis.Diagnostics;
using System.Collections.Immutable;

namespace UnityScriptAnalyzer.Analyzers.MemberAccessAnalyzers
{
    /// <summary>
    /// This analyzer searches for member access of the Tag property of Unity GameObject. Accessing string parameters in Unity GameObject requires crossing the native/managed boundary which is not cheap
    /// Generally, the Tag property is accessed for comparison. Unity provides the CompareTag() function will is more performant than doing a generic equals comparison with the Tag property
    /// </summary>
    [DiagnosticAnalyzer(LanguageNames.CSharp)]
    public class TagComparisonAnalyzer : DiagnosticAnalyzer
    {
        private static readonly string Message = "Use CompareTag() to make a string comparison with GameObject.Tag or Component.Tag";

        private static readonly string Title = "GameObject.Tag Comparison";

        internal static DiagnosticDescriptor Rule
            => new DiagnosticDescriptor(DiagnosticIDs.CompareTag, Title, Message, Consts.Category, DiagnosticSeverity.Warning, true);

        public override ImmutableArray<DiagnosticDescriptor> SupportedDiagnostics => ImmutableArray.Create(Rule);

        public override void Initialize(AnalysisContext context)
        {
            context.RegisterSyntaxNodeAction(AnalyzeInvocation, SyntaxKind.InvocationExpression);

            // We only care about == or != string comparisons (i.e GameObject.tag == "test")
            context.RegisterSyntaxNodeAction(AnalyzeBinaryExpression, new[] { SyntaxKind.EqualsExpression, SyntaxKind.NotEqualsExpression });
        }

        private static void AnalyzeInvocation(SyntaxNodeAnalysisContext context)
        {
            var node = context.Node as InvocationExpressionSyntax;

            // Equals only takes one parameter, any other functions can be ignored
            if (node.ArgumentList?.Arguments.Count != 1)
            {
                return;
            }

            var methodSymbol = context.SemanticModel.GetSymbolInfo(node).Symbol as IMethodSymbol;
            if (methodSymbol == null)
                return;

            string methodName = methodSymbol.ToString();

            // If the method is not equals, then return and skip analysis
            if (!methodName.Equals("string.Equals(string)"))
            {
                return;
            }

            // This checks the left side of the equals method call (i.e GameObject.tag.Equals)
            var access = node.Expression as MemberAccessExpressionSyntax;
            if (access != null)
            {
                if (IsGameObjectTagProperty(ref context, access.Expression))
                { 
                    Helpers.CreateAndReportDiagnostic(Rule, node.GetLocation(), ref context);
                    return;
                }
            }

            // We know there is at least one argument from check above
            // Thus, this checks the right side of the method call (i.e string.Equals(GameObject.Tag))
            var argument = node.ArgumentList.Arguments[0].Expression;
            if (IsGameObjectTagProperty(ref context, argument))
            {
                Helpers.CreateAndReportDiagnostic(Rule, node.GetLocation(), ref context);
                return;
            }
        }

        private static void AnalyzeBinaryExpression(SyntaxNodeAnalysisContext context)
        {
            var node = context.Node as BinaryExpressionSyntax;

            if (IsGameObjectTagProperty(ref context, node.Left))
            {
                Helpers.CreateAndReportDiagnostic(Rule, node.GetLocation(), ref context);
            }
            else if (IsGameObjectTagProperty(ref context, node.Right))
            {
                Helpers.CreateAndReportDiagnostic(Rule, node.GetLocation(), ref context);
            }
        }

        /// <summary>
        ///  This helper checks whether the passed node is the GameObject.Tag type
        /// </summary>
        /// <param name="context">Analyzer context</param>
        /// <param name="node">node to analyze for GameObject.Tag</param>
        /// <returns>true if it is GameObject.tag, false otherwise</returns>
        private static bool IsGameObjectTagProperty(ref SyntaxNodeAnalysisContext context, ExpressionSyntax node)
        {
            const string GameObjectTag = "UnityEngine.GameObject.tag";
            const string ComponentTag = "UnityEngine.Component.tag";
            var prop = context.SemanticModel.GetSymbolInfo(node).Symbol as IPropertySymbol;
            if (prop != null)
            {
                return GameObjectTag.Equals(prop.ToString()) || ComponentTag.Equals(prop.ToString());
            }

            return false;
        }
    }
}
