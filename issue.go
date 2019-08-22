package main

import (
	"github.com/xanzy/go-gitlab"
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
