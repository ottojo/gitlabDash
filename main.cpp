#include <iostream>
#include "issue/Issue.h"
#include <mstch/mstch.hpp>
#include <fstream>

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <httplib.h>


std::shared_ptr<mstch::map> createPage(const std::vector<Issue> &issues) {
    auto page = std::make_shared<mstch::map>();

    mstch::array issueArray;

    for (const Issue &issue: issues) {
        mstch::array labels;
        for (std::string label : issue.getLabels()) {
            labels.push_back(mstch::map{{"name", label}});
        }

        issueArray.push_back(mstch::map{{"title",       issue.getTitle()},
                                        {"description", issue.getDescription()},
                                        {"author",      issue.getAuthor()},
                                        {"assignee",    issue.getAssignee()},
                                        {"dueDate",     issue.getDueDate()},
                                        {"isOverdue",   issue.isOverdue()},
                                        {"labels",      labels}});
    }
    (*page)["issues"] = issueArray;

    return page;
}

int main(int argc, char *argv[]) {

    // first argument: port
    if (argc < 2) {
        std::cerr << "please specify port" << std::endl;
    }

    // https://git.spatz.wtf/spatzenhirn/2020/issues.atom?feed_token=eJ7JpgE3Y4GcbBfJsQaW&state=opened
    // https://git.spatz.wtf/spatzenhirn/2020/issues.atom?feed_token=eJ7JpgE3Y4GcbBfJsQaW&state=closed

    std::ifstream ifs("index.html");
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));


    httplib::Server srv;

    srv.Get("/", [content](const httplib::Request &req, httplib::Response &res) {

        httplib::SSLClient cli("git.spatz.wtf", 443);
        //TODO fix ssl
        cli.enable_server_certificate_verification(false);

        auto response = cli.Get("/spatzenhirn/2020/issues.atom?feed_token=eJ7JpgE3Y4GcbBfJsQaW&state=opened");
        if (!response || response->status != 200) {
            std::cerr << "Error getting RSS feed" << std::endl;
            return;
        }

        std::vector<Issue> openIssues = Issue::ParseIssues(response->body);

        auto page = createPage(openIssues);

        res.set_content(mstch::render(content, *page), "text/html");
    });

    srv.listen("0.0.0.0", std::stoi(argv[1]));

    return 0;
}