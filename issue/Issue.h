/**
 * @file Issue.h
 * @author jonas
 * @date 8/19/19
 * Description here TODO
 */

#ifndef DASHSERVER_ISSUE_H
#define DASHSERVER_ISSUE_H


#include <chrono>
#include <mstch/mstch.hpp>
#include "../person/Person.h"

class Issue {
public:
    Issue(std::string id, std::string link, std::string title,
          std::chrono::system_clock::time_point lastUpdated, Person author, std::string summary,
          std::string description, std::vector<std::string> labels,
          std::vector<Person> assignees, Person assignee, std::optional<std::chrono::system_clock::time_point> dueDate);

    [[nodiscard]] const std::string &getTitle() const;

    [[nodiscard]] const std::string &getAuthor() const;

    [[nodiscard]] std::string getDueDate() const;

    [[nodiscard]] const std::string &getAssignee() const;

    [[nodiscard]] const std::string &getDescription() const;

    [[nodiscard]] bool isOverdue() const;

    static std::vector<Issue> ParseIssues(const std::string &xml);

private:
    std::string id;
    std::string link;
    std::string title;
    std::chrono::system_clock::time_point lastUpdated;
    Person author;
    std::string summary;
    std::string description;
    std::vector<std::string> labels;
    std::vector<Person> assignees;
    Person assignee;
    std::optional<std::chrono::system_clock::time_point> dueDate;
};


#endif //DASHSERVER_ISSUE_H
