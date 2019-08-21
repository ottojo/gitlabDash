package main

import (
	"github.com/xanzy/go-gitlab"
)

type MergeRequestPage struct {
	MergeRequests []MergeRequest
}

type MergeRequest gitlab.MergeRequest

func (m *MergeRequest) ColorClasses() string {
	for _, label := range m.Labels {
		switch label {
		case "Needs C++ Review":
			return "text-white bg-primary"
		case "Changes Requested":
			return "text-white bg-info"
		case "Ready To Merge":
			return "text-white bg-success"
		}
	}
	return ""
}
