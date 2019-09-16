package main

import (
	"github.com/microcosm-cc/bluemonday"
	"github.com/xanzy/go-gitlab"
	"gopkg.in/russross/blackfriday.v2"
	"html/template"
)

type MergeRequestPage struct {
	MergeRequests []MergeRequest
}

type MergeRequest gitlab.MergeRequest

func (m *MergeRequest) ColorClasses() string {
	//  TODO: fix code and link colors to be readable on non-white background
	for _, label := range m.Labels {
		switch label {
		case "Needs C++ Review":
			return ""
		case "Changes Requested":
			return ""
		case "Ready To Merge":
			return ""
		}
	}
	return ""
}

func (m *MergeRequest) GetDescriptionHTML() template.HTML {
	unsafeHTMLfromMarkdown := blackfriday.Run([]byte(m.Description))
	return template.HTML(bluemonday.UGCPolicy().SanitizeBytes(unsafeHTMLfromMarkdown))
}
