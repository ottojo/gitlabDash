package main

import "github.com/xanzy/go-gitlab"

type EventsPage struct {
	Events []Event
}

type Event struct {
	Event   gitlab.ContributionEvent
	Project *gitlab.Project
}
