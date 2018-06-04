using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.Diagnostics;
using System;
using System.Collections.Generic;
using System.Text;

namespace UnityScriptAnalyzer
{
    public class Helpers
    {
        public static void CreateAndReportDiagnostic(DiagnosticDescriptor rule, Location loc, ref SyntaxNodeAnalysisContext context, params object[] paramArgs)
        {
            var diagnostic = Diagnostic.Create(rule, loc, paramArgs);
            context.ReportDiagnostic(diagnostic);
        }
    }
}
