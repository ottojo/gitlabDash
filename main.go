package main

import (
	"flag"
	"github.com/gin-gonic/autotls"
	"github.com/gin-gonic/gin"
	"github.com/xanzy/go-gitlab"
	"log"
	"net/http"
)

func main() {

	baseUrl := flag.String("baseUrl", "https://gitlab.com/api/v4", "Base API URL")
	projectName := flag.String("projectName", "spatzenhirn/2020", "GitLab projectName")
	token := flag.String("token", "", "Personal Access Token")
	domain := flag.String("domain", "dash.otto.cool", "Domain name for TLS certificate")
	flag.Parse()

	git := gitlab.NewClient(nil, *token)
	err := git.SetBaseURL(*baseUrl)
	if err != nil {
		log.Fatal(err)
	}

	router := gin.Default()
	router.LoadHTMLFiles("issues.html", "mergeRequests.html")

	router.GET("/issues", func(c *gin.Context) {

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

	router.GET("/mergeRequests", func(c *gin.Context) {

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

	go log.Fatal(http.ListenAndServe(":http", http.HandlerFunc(redirect)))

	log.Fatal(autotls.Run(router, *domain))
}

func redirect(w http.ResponseWriter, req *http.Request) {
	target := "https://" + req.Host + req.URL.Path
	http.Redirect(w, req, target, http.StatusMovedPermanently)
}
