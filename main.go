package main

import (
	"flag"
	"github.com/gin-gonic/gin"
	"github.com/xanzy/go-gitlab"
	"log"
	"net/http"
)

func main() {

	baseUrl := flag.String("baseUrl", "https://gitlab.com/api/v4", "Base API URL")
	projectName := flag.String("projectName", "spatzenhirn/2020", "GitLab projectName")
	token := flag.String("token", "", "Personal Access Token")
	flag.Parse()

	git := gitlab.NewClient(nil, *token)
	err := git.SetBaseURL(*baseUrl)
	if err != nil {
		log.Fatal(err)
	}

	r := gin.Default()
	r.LoadHTMLFiles("issues.html", "mergeRequests.html")

	r.GET("/issues", func(c *gin.Context) {

		gitIssues, _, err := git.Issues.ListProjectIssues(*projectName, &gitlab.ListProjectIssuesOptions{})
		if err != nil {
			_ = c.Error(err)
			return
		}
		var issues []Issue
		for _, issue := range gitIssues {
			if issue.State != "closed" {
				issues = append(issues, Issue(*issue))
			}
		}

		c.HTML(http.StatusOK, "issues.html", IssuePage{Issues: issues})
	})

	r.GET("/mergeRequests", func(c *gin.Context) {

		mergeRequests, _, err := git.MergeRequests.ListProjectMergeRequests(*projectName, &gitlab.ListProjectMergeRequestsOptions{})
		if err != nil {
			log.Fatal(err)
		}
		var requests []MergeRequest
		for _, mr := range mergeRequests {
			if mr.State == "opened" {
				requests = append(requests, MergeRequest(*mr))
			}
		}

		c.HTML(http.StatusOK, "mergeRequests.html", MergeRequestPage{MergeRequests: requests})
	})

	log.Fatal(r.Run())

}
