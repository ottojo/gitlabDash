package main

import (
	"flag"
	"github.com/gin-gonic/gin"
	"github.com/xanzy/go-gitlab"
	"golang.org/x/crypto/acme/autocert"
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

	m := autocert.Manager{
		Prompt:     autocert.AcceptTOS,
		HostPolicy: autocert.HostWhitelist("dash.otto.cool"),
		Cache:      autocert.DirCache("/var/www/.cache"),
	}

	s := &http.Server{
		Addr:      ":https",
		TLSConfig: m.TLSConfig(),
		Handler:   r,
	}

	go log.Fatal(http.ListenAndServe(":http", m.HTTPHandler(http.HandlerFunc(redirect))))

	log.Fatal(s.ListenAndServeTLS("", ""))
}

func redirect(w http.ResponseWriter, req *http.Request) {
	target := "https://" + req.Host + req.URL.Path
	http.Redirect(w, req, target, http.StatusMovedPermanently)
}
