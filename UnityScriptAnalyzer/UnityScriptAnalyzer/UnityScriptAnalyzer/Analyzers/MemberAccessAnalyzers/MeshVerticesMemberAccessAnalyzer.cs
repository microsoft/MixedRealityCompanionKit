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
    /// This analyzer searches for element access of the Mesh.Vertices property. This Unity component returns a copy of the vertices array.
    /// Thus, if a developer attempts to index into this property, they will receive an array allocation/copy every attempt which is extremely costly
    /// </summary>
    [DiagnosticAnalyzer(LanguageNames.CSharp)]
    public class MeshVerticesMemberAccessAnalyzer : DiagnosticAnalyzer
    {
        private static readonly string Message = "Mesh.Vertices returns an array copy of the data. Consider saving pointer reference to data if performing multiple access";

        private static readonly string Title = "Mesh.Vertices";

        internal static DiagnosticDescriptor MeshVerticesRules = new DiagnosticDescriptor(DiagnosticIDs.MeshVertices,
                                                            Title,
                                                            Message,
                                                            Consts.Category,
                                                            DiagnosticSeverity.Warning,
                                                            isEnabledByDefault: true);

        public override ImmutableArray<DiagnosticDescriptor> SupportedDiagnostics => ImmutableArray.Create(MeshVerticesRules);

        public override void Initialize(AnalysisContext context) => context.RegisterSyntaxNodeAction(Analyze, SyntaxKind.ElementAccessExpression);

        private static void Analyze(SyntaxNodeAnalysisContext context)
        {
            var identifer = (ElementAccessExpressionSyntax)context.Node;

            if (identifer == null)
            {
                return;
            }

            var memberSymbol = context.SemanticModel.GetSymbolInfo(identifer.Expression).Symbol;

            if (memberSymbol == null)
                return;

            string name = memberSymbol.ToString();

            // If this member access is the Unity type we care about, then create a diagnostic warning the user
            if (name.StartsWith("UnityEngine.Mesh.vertices"))
            {
                Helpers.CreateAndReportDiagnostic(MeshVerticesRules, identifer.GetLocation(), ref context);
            }
        }
    }
}
