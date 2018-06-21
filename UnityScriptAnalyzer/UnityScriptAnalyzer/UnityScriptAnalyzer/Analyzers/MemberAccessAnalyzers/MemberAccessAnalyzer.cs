// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Microsoft.CodeAnalysis.Diagnostics;
using System.Collections.Immutable;
using System.Linq;

namespace UnityScriptAnalyzer.Analyzers.MemberAccessAnalyzers
{
    /// <summary>
    /// This analyzer search member access operations in code for two rules:
    /// 1) Similar to Vector.Distance(), the Vector magnitude member also involves a sqrt() operation when accessing this property. sqrMangitude should be used instead to avoid the additional sqrt() load
    /// 2) DateTime.Now is much more expensive than DateTime.UtcNow due to the additional operations to lookup and adjust for local timezone. If truly needed, DateTime.UtcNow should ideally be used for performance
    /// </summary>
    [DiagnosticAnalyzer(LanguageNames.CSharp)]
    public class MemberAccessAnalyzer : DiagnosticAnalyzer
    {
        private const DiagnosticSeverity Severity = DiagnosticSeverity.Warning;

        private static readonly string MagnitudeMessage = "Consider using sqrMagnitude if applicable instead of magnitude in order to eliminate expensive sqrt calculations";

        private static readonly string MagnitudeTitle = "Vector magnitude";

        internal static DiagnosticDescriptor VectorMagRule = new DiagnosticDescriptor(DiagnosticIDs.VectorMagnitude,
                                                            MagnitudeTitle,
                                                            MagnitudeMessage,
                                                            Consts.Category,
                                                            DiagnosticSeverity.Warning,
                                                            isEnabledByDefault: true);

        private static readonly string DateTimeNowMessage = "Consider using less expensive DateTime.UtcNow instead of DateTime.Now if applicable";

        private static readonly string DateTimeNowTitle = "DateTime Now";

        internal static DiagnosticDescriptor DateTimeNowRule = new DiagnosticDescriptor(DiagnosticIDs.DateTimeNow,
                                                                    DateTimeNowTitle,
                                                                    DateTimeNowMessage,
                                                                    Consts.Category,
                                                                    DiagnosticSeverity.Warning,
                                                                    isEnabledByDefault: true);
        public override ImmutableArray<DiagnosticDescriptor> SupportedDiagnostics => ImmutableArray.Create(VectorMagRule, DateTimeNowRule);

        public override void Initialize(AnalysisContext context) => context.RegisterSyntaxNodeAction(Analyze, SyntaxKind.SimpleMemberAccessExpression);

        private static string[] s_MemberAccessNames = { "magnitude", "Now" };

        private static void Analyze(SyntaxNodeAnalysisContext context)
        {
            var identifer = (MemberAccessExpressionSyntax)context.Node;

            // The member access here must match one of the names we are searching for. 
            // This simple string check ensure we do not waste processing looking up the member's symbol information
            if (identifer == null || !s_MemberAccessNames.Contains(identifer.Name.ToString()))
            {
                return;
            }

            var memberSymbol = context.SemanticModel.GetSymbolInfo(identifer.Name).Symbol;

            if (memberSymbol == null)
                return;

            string name = memberSymbol.ToString();

            // Check that the member access is of the Unity type we care about, and if so create a diagnostic
            if (name.StartsWith("UnityEngine.Vector3.magnitude") || name.StartsWith("UnityEngine.Vector2.magnitude"))
            {
                Helpers.CreateAndReportDiagnostic(VectorMagRule, identifer.GetLocation(), ref context);
            }
            else if (name.StartsWith("System.DateTime.Now"))
            {
                Helpers.CreateAndReportDiagnostic(DateTimeNowRule, identifer.GetLocation(), ref context);
            }
        }
    }
}
