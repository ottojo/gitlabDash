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

    std::string getTitle() const;

    std::string getAuthor() const;

    std::string getAge() const;

    std::string getAssignee() const;

    std::string getDescription() const;

    static std::vector<Issue> ParseIssues(const std::string &xml);
};


#endif //DASHSERVER_ISSUE_H
