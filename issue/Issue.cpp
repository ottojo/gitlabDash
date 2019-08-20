/**
 * @file Issue.cpp
 * @author jonas
 * @date 8/19/19
 * Description here TODO
 */

#include <tinyxml2.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include "Issue.h"

std::vector<Issue> Issue::ParseIssues(const std::string &xml) {

    std::vector<Issue> issues;

    tinyxml2::XMLDocument doc;
    doc.Parse(xml.c_str());

    auto feed = doc.FirstChildElement();

    for (auto entry = feed->FirstChildElement("entry");
         entry != nullptr; entry = entry->NextSiblingElement("entry")) {

        Issue issue;

        issue.id = entry->FirstChildElement("id")->GetText();
        issue.link = entry->FirstChildElement("link")->Attribute("href");
        issue.title = entry->FirstChildElement("title")->GetText();

        // TODO timezones?
        std::tm tm = {};
        std::stringstream(entry->FirstChildElement("updated")->GetText()) >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
        issue.lastUpdated = std::chrono::system_clock::from_time_t(std::mktime(&tm));

        auto author = entry->FirstChildElement("author");
        issue.author.name = author->FirstChildElement("name")->GetText();
        auto authoremail = author->FirstChildElement("email");
        if (authoremail->FirstChildElement("email") != nullptr) {
            issue.author.email = authoremail->GetText();

        }

        issue.summary = entry->FirstChildElement("summary")->GetText();

        if (entry->FirstChildElement("description")->GetText() != nullptr) {
            issue.description = entry->FirstChildElement("description")->GetText();
        }


        auto labels = entry->FirstChildElement("labels");
        if (labels != nullptr) {
            for (auto label = labels->FirstChildElement("label");
                 label != nullptr; label = label->NextSiblingElement("label")) {
                issue.labels.emplace_back(label->GetText());
            }
        }

        auto assignees = entry->FirstChildElement("assignees");

        for (auto assignee = assignees->FirstChildElement("assignee");
             assignee != nullptr; assignee = assignee->NextSiblingElement("assignees")) {
            Person newAssignee;
            newAssignee.name = assignee->FirstChildElement("name")->GetText();
            if (assignee->FirstChildElement("email")->GetText() != nullptr) {
                newAssignee.email = assignee->FirstChildElement("email")->GetText();
            }
            issue.assignees.push_back(newAssignee);
        }

        auto assignee = entry->FirstChildElement("assignee");
        issue.assignee.name = assignee->FirstChildElement("name")->GetText();
        if (assignee->FirstChildElement("email")->GetText() != nullptr) {
            issue.assignee.email = assignee->FirstChildElement("email")->GetText();
        }

        issues.push_back(issue);
    }

    return issues;
}


std::string Issue::getTitle() const {
    return title;
}

std::string Issue::getAuthor() const {
    return author.name;
}

std::string Issue::getDescription() const {
    return description;
}

std::string Issue::getAssignee() const {
    return assignee.name;
}

std::string Issue::getAge() const {
    std::time_t t = std::chrono::system_clock::to_time_t(lastUpdated);
    return std::ctime(&t);

}
