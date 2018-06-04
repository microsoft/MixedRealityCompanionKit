// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Microsoft.CodeAnalysis.Diagnostics;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Linq;

namespace UnityScriptAnalyzer.Analyzers
{
    [DiagnosticAnalyzer(LanguageNames.CSharp)]
    public class LoopInstantiationAnalyzer : DiagnosticAnalyzer
    {
        private const DiagnosticSeverity Severity = DiagnosticSeverity.Warning;

        private static readonly string Message = "Pass parent transform for variable \"{0}\" to original GameObject.Instantiate() call. See GameObject.Instantiate() overloads. Also, consider setting hierarchyCapacity for \"{1}\" if number of children is known";

        private static readonly string Title = "Parent Trasnform for GameObject.Instantiate()";

        internal static DiagnosticDescriptor Rule
            => new DiagnosticDescriptor(Consts.DiagnosticId, Title, Message, Consts.Category, Severity, true);

        public override ImmutableArray<DiagnosticDescriptor> SupportedDiagnostics => ImmutableArray.Create(Rule);

        public override void Initialize(AnalysisContext context)
        {
            context.RegisterSyntaxNodeAction(Analyze, SyntaxKind.ForEachStatement);
            context.RegisterSyntaxNodeAction(Analyze, SyntaxKind.ForStatement);
        }

        private static void Analyze(SyntaxNodeAnalysisContext context)
        {
            var loopRoot = context.Node;            

            foreach(var currentNode in loopRoot.DescendantNodes())
            {
                if (currentNode is VariableDeclarationSyntax || currentNode is AssignmentExpressionSyntax)
                {
                    if (ContainsInstantiation(currentNode.DescendantNodes().OfType<InvocationExpressionSyntax>(), context))
                    {
                        string varName;
                        if (currentNode is AssignmentExpressionSyntax)
                        {
                            varName = (currentNode as AssignmentExpressionSyntax).Left.ToString();
                        }
                        else
                        {
                            var variable = currentNode.DescendantNodes().OfType<VariableDeclaratorSyntax>().FirstOrDefault();
                            varName = variable.Identifier.ToString(); // variable name
                        }

                        int searchStart = currentNode.Span.End;

                        // Look for AssignmentSyntax nodes where our variable name above could be called with .trasnform.parent
                        var candidates = loopRoot.DescendantNodes().OfType<AssignmentExpressionSyntax>().Where(s => s.SpanStart > searchStart);

                        string searchStr = varName + ".transform.parent";
                        foreach (AssignmentExpressionSyntax candidate in candidates)
                        {
                            if (candidate.Left.ToString().Equals(searchStr))
                            {
                                var parentTransform = candidate.Right.ToString();
                                Helpers.CreateAndReportDiagnostic(Rule, candidate.GetLocation(), ref context, varName, parentTransform);
                            }
                        }
                    }
                }
            }
        }

        private static bool ContainsInstantiation(IEnumerable<InvocationExpressionSyntax> methods, SyntaxNodeAnalysisContext context)
        {
            foreach (var method in methods)
            {
                var methodSymbol = context.SemanticModel.GetSymbolInfo(method).Symbol as IMethodSymbol;

                if (methodSymbol == null)
                    break;

                var methodName = methodSymbol.ToString();

                // TODO: check for transform parent overload?
                if (methodName.StartsWith("UnityEngine.Object.Instantiate"))
                    return true;
            }

            return false;
        }
    }
}
