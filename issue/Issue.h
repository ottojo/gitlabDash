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

public:

    [[nodiscard]] std::string getTitle() const;

    [[nodiscard]] std::string getAuthor() const;

    [[nodiscard]] std::string getAge() const;

    [[nodiscard]] std::string getAssignee() const;

    [[nodiscard]] std::string getDescription() const;

    static std::vector<Issue> ParseIssues(const std::string &xml);
};


#endif //DASHSERVER_ISSUE_H
