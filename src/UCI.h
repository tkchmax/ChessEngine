#pragma once
#ifndef UCI_H_
#define UCI_H_
#include <vector>
#include <string>
#include <unordered_map>
#include "Constants.h"

class UCI
{
public:
    struct Flags {
        Flags() :
            timeset(false),
            time(0),
            starttime(0),
            stoptime(0),
            movetime(0)
        {}
        bool timeset;
        long long time;
        long long movetime;
        long long starttime, stoptime;
    };
    static void init();
    static std::vector<std::string> parse(std::string cmd);
    static void loop();
    static long long get_time_ms();
    virtual std::vector<std::string> handle(std::vector<std::string> args) = 0;
public:
    static UCI::Flags flags;
protected:
    static std::unordered_map<std::string, UCI*> uci;
};


class UCI_ISREADY : public UCI {
    virtual std::vector<std::string> handle(std::vector<std::string> args) override;
};

class UCI_UCI : public UCI {
    virtual std::vector<std::string> handle(std::vector<std::string> args) override;
};

class UCI_POSITION : public UCI {
    virtual std::vector<std::string> handle(std::vector<std::string> args) override;
};

class UCI_DRAW : public UCI {
    virtual std::vector<std::string> handle(std::vector<std::string> args) override;
};

class UCI_GO : public UCI {
    virtual std::vector<std::string> handle(std::vector<std::string> args) override;
};

#endif //UCI_H_

