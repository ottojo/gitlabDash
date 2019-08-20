/**
 * @file Person.h
 * @author jonas
 * @date 8/19/19
 * Description here TODO
 */

#ifndef DASHSERVER_PERSON_H
#define DASHSERVER_PERSON_H


#include <string>
#include <utility>

class Person {
public:
    Person(std::string n, std::string e) : name(std::move(n)), email(std::move(e)) {};

    [[nodiscard]] const std::string &getName() const;

    [[nodiscard]] const std::string &getEmail() const;

private:
    std::string name;
    std::string email;
};


#endif //DASHSERVER_PERSON_H
