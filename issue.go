package main

import (
	"github.com/microcosm-cc/bluemonday"
	"github.com/xanzy/go-gitlab"
	"gopkg.in/russross/blackfriday.v2"
	"html/template"
	"time"
)

type IssuePage struct {
	Issues []Issue
}

type Issue gitlab.Issue

func (i *Issue) IsOverdue() bool {
	if i.DueDate == nil {
		return false
	}
	return time.Now().After(time.Time(*i.DueDate))
}

func (i *Issue) GetDescriptionHTML() template.HTML {
	unsafeHTMLfromMarkdown := blackfriday.Run([]byte(i.Description))
	return template.HTML(bluemonday.UGCPolicy().SanitizeBytes(unsafeHTMLfromMarkdown))
}
