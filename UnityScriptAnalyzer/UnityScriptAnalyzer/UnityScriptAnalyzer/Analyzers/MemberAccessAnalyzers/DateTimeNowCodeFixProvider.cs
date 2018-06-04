// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CodeActions;
using Microsoft.CodeAnalysis.CodeFixes;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using System.Collections.Immutable;
using System.Composition;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using static Microsoft.CodeAnalysis.CSharp.SyntaxFactory;

namespace UnityScriptAnalyzer.Analyzers.MemberAccessAnalyzers
{
    /// <summary>
    /// This class provides code to fix the DateTimeNow diagnostic (USA0005) by replacing DateTime.Now with DateTime.UtcNow
    /// </summary>
    [ExportCodeFixProvider(LanguageNames.CSharp, Name = nameof(DateTimeNowCodeFixProvider)), Shared]
    public class DateTimeNowCodeFixProvider : CodeFixProvider
    {
        private const string title = "Change DateTime.Now to DateTime.UtcNow";

        public sealed override ImmutableArray<string> FixableDiagnosticIds
        {
            get { return ImmutableArray.Create(DiagnosticIDs.DateTimeNow); }
        }

        public sealed override FixAllProvider GetFixAllProvider()
        {
            // See https://github.com/dotnet/roslyn/blob/master/docs/analyzers/FixAllProvider.md for more information on Fix All Providers
            return WellKnownFixAllProviders.BatchFixer;
        }

        public sealed override async Task RegisterCodeFixesAsync(CodeFixContext context)
        {
            var diagnostic = context.Diagnostics.First();

            context.RegisterCodeFix(CodeAction.Create(
                title: title,
                createChangedDocument: token => RenameDateTimeNow(context.Document, diagnostic, token)),
                diagnostic);
        }

        private static async Task<Document> RenameDateTimeNow(Document document, Diagnostic diagnostic, CancellationToken cancellationToken)
        {
            var root = await document.GetSyntaxRootAsync(cancellationToken).ConfigureAwait(false);
            var diagnosticSpan = diagnostic.Location.SourceSpan;

            // Use information in diagnostic created to find our DateTime.Now member access node
            var memberAccess = root.FindNode(diagnosticSpan) as MemberAccessExpressionSyntax;

            // Replace the .Now token with the .UtcNow token
            var newMemberAccess = memberAccess.WithName(IdentifierName("UtcNow"));

            return document.WithSyntaxRoot(root.ReplaceNode(memberAccess, newMemberAccess));           
        }
    }
}
