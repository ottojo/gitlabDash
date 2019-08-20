/**
 * @file Issue.cpp
 * @author jonas
 * @date 8/19/19
 * Description here TODO
 */

#include <tinyxml2.h>
#include <iomanip>
#include <utility>
#include <vector>
#include "Issue.h"

std::vector<Issue> Issue::ParseIssues(const std::string &xml) {

    std::vector<Issue> issues;

    tinyxml2::XMLDocument doc;
    doc.Parse(xml.c_str());

    auto feed = doc.FirstChildElement();

    for (auto entry = feed->FirstChildElement("entry");
         entry != nullptr; entry = entry->NextSiblingElement("entry")) {


        std::string id = entry->FirstChildElement("id")->GetText();
        std::string link = entry->FirstChildElement("link")->Attribute("href");
        std::string title = entry->FirstChildElement("title")->GetText();

        // TODO timezones?
        std::tm tm = {};
        std::stringstream(entry->FirstChildElement("updated")->GetText()) >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
        auto lastUpdated = std::chrono::system_clock::from_time_t(std::mktime(&tm));

        auto authorElement = entry->FirstChildElement("author");
        std::string authorName = authorElement->FirstChildElement("name")->GetText();
        auto authoremailElement = authorElement->FirstChildElement("email");
        std::string authorEmail;
        if (authoremailElement->FirstChildElement("email") != nullptr) {
            authorEmail = authoremailElement->GetText();

        }
        Person author(authorName, authorEmail);

        std::string summary = entry->FirstChildElement("summary")->GetText();

        std::string description;
        if (entry->FirstChildElement("description")->GetText() != nullptr) {
            description = entry->FirstChildElement("description")->GetText();
        }

        auto labels = entry->FirstChildElement("labels");
        std::vector<std::string> labelsList;
        if (labels != nullptr) {
            for (auto label = labels->FirstChildElement("label");
                 label != nullptr; label = label->NextSiblingElement("label")) {
                labelsList.emplace_back(label->GetText());
            }
        }

        auto assignees = entry->FirstChildElement("assignees");

        std::vector<Person> assigneeList;
        for (auto assignee = assignees->FirstChildElement("assignee");
             assignee != nullptr; assignee = assignee->NextSiblingElement("assignees")) {
            std::string name = assignee->FirstChildElement("name")->GetText();
            std::string email;
            if (assignee->FirstChildElement("email")->GetText() != nullptr) {
                email = assignee->FirstChildElement("email")->GetText();
            }
            assigneeList.emplace_back(name, email);
        }

        auto assigneeElement = entry->FirstChildElement("assignee");
        std::string assigneeName = assigneeElement->FirstChildElement("name")->GetText();
        std::string assigneeEmail;
        if (assigneeElement->FirstChildElement("email")->GetText() != nullptr) {
            assigneeEmail = assigneeElement->FirstChildElement("email")->GetText();
        }
        Person assignee(assigneeName, assigneeEmail);

        issues.emplace_back(id, link, title, lastUpdated, author, summary, description, labelsList, assigneeList,
                            assignee);
    }

    return issues;
}


const std::string &Issue::getTitle() const {
    return title;
}

const std::string &Issue::getAuthor() const {
    return author.getName();
}

const std::string &Issue::getDescription() const {
    return description;
}

const std::string &Issue::getAssignee() const {
    return assignee.getName();
}

std::string Issue::getAge() const {
    std::time_t t = std::chrono::system_clock::to_time_t(lastUpdated);
    return std::ctime(&t);
}

Issue::Issue(std::string id, std::string link, std::string title,
             const std::chrono::system_clock::time_point &lastUpdated, Person author, std::string summary,
             std::string description, std::vector<std::string> labels,
             std::vector<Person> assignees, Person assignee) : id(std::move(id)), link(std::move(link)),
                                                               title(std::move(title)),
                                                               lastUpdated(lastUpdated),
                                                               author(std::move(author)),
                                                               summary(std::move(summary)),
                                                               description(std::move(description)),
                                                               labels(std::move(labels)),
                                                               assignees(std::move(assignees)),
                                                               assignee(std::move(assignee)) {}
