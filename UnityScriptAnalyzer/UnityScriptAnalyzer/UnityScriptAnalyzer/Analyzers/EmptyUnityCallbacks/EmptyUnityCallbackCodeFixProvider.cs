// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CodeActions;
using Microsoft.CodeAnalysis.CodeFixes;
using System.Collections.Immutable;
using System.Composition;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

namespace UnityScriptAnalyzer.Analyzers.EmptyUnityCallbacks
{
    /// <summary>
    /// This class provides code to fix EmptyUnityCallback(USA0001) issues by removing the empty callback function altogether
    /// </summary>
    [ExportCodeFixProvider(LanguageNames.CSharp, Name = nameof(EmptyUnityCallbackCodeFixProvider)), Shared]
    public class EmptyUnityCallbackCodeFixProvider : CodeFixProvider
    {
        private const string title = "Remove empty code block";

        public sealed override ImmutableArray<string> FixableDiagnosticIds
        {
            get { return ImmutableArray.Create(DiagnosticIDs.EmptyUnityCallback); }
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
                createChangedDocument: token => RemoveUnityCallback(context.Document, diagnostic, token)), 
                diagnostic);
        }

        private static async Task<Document> RemoveUnityCallback(Document document, Diagnostic diagnostic, CancellationToken cancellationToken)
        {
            var root = await document.GetSyntaxRootAsync(cancellationToken).ConfigureAwait(false);
            var diagnosticSpan = diagnostic.Location.SourceSpan;

            // Find the method declaration using information in our logged diagnostic (i.e the method location in the document)
            var methodDeclaration = root.FindToken(diagnosticSpan.Start).Parent;

            // Remove the document without this unity method that has an empty body
            return document.WithSyntaxRoot(root.RemoveNode(methodDeclaration, SyntaxRemoveOptions.KeepNoTrivia));
        }
    }
}
