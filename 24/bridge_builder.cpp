#include <bridge_builder.hpp>

#include <algorithm>
#include <cassert>
#include <functional>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>

Ports parseInput(std::string_view input)
{
    Ports ret;

    std::regex rx_line(R"(^(\d+)/(\d+)$)");
    std::stringstream sstr(std::string{input});
    std::string line;
    while(std::getline(sstr, line)) {
        std::smatch matches;
        std::regex_match(line, matches, rx_line);
        ret.push_back(Port{ std::stoi(matches[1]), std::stoi(matches[2]) });
    }

    return ret;
}

template<typename Func>
void iteratePorts(Ports const& ports, Ports taken, int searched, Func&& cb)
{
    auto const it_end = cend(ports);
    for(auto it=cbegin(ports); it != it_end; ++it) {
        bool const first_matches = ((*it)[0] == searched);
        bool const second_matches = ((*it)[1] == searched);
        if((first_matches || second_matches) &&
           (std::find(begin(taken), end(taken), *it) == end(taken)))
        {
            cb(*it, (first_matches ? (*it)[1] : (*it)[0]));
        }
    }
}

struct IteratePorts_rec {
    Ports const* m_ports;
    Ports* m_currentBridge;
    std::vector<Ports>* m_bridges;

    IteratePorts_rec(Ports const& ports, Ports& current_bridge, std::vector<Ports>& bridges)
        :m_ports(&ports), m_currentBridge(&current_bridge), m_bridges(&bridges)
    {}

    void operator()(Port p, int other)
    {
        m_currentBridge->push_back(p);
        IteratePorts_rec cb(*m_ports, *m_currentBridge, *m_bridges);
        iteratePorts(*m_ports, *m_currentBridge, other, cb);
        m_bridges->push_back(*m_currentBridge);
        m_currentBridge->pop_back();
    }
};

std::vector<Ports> getBridges(Ports const& ports)
{
    std::vector<Ports> ret;
    Ports current_bridge;
    IteratePorts_rec cb(ports, current_bridge, ret);
    iteratePorts(ports, current_bridge, 0, cb);

    return ret;
}

int getBridgeStrength(Ports const& bridge)
{
    return std::accumulate(begin(bridge), end(bridge), 0,
                           [](int acc, Port p) { return acc + p[0] + p[1]; });
}

int getStrongestStrength(std::vector<Ports> const& bridges)
{
    std::vector<int> strengths(bridges.size(), 0);
    std::transform(begin(bridges), end(bridges), begin(strengths), getBridgeStrength);
    return *std::max_element(begin(strengths), end(strengths));
}

int getLongestStrongestStrength(std::vector<Ports> const& bridges)
{
    std::size_t const longest = std::max_element(begin(bridges), end(bridges),
                                                 [](Ports const& b1, Ports const& b2) { return b1.size() < b2.size(); })->size();
    std::vector<Ports> long_bridges;
    std::copy_if(begin(bridges), end(bridges), std::back_inserter(long_bridges),
                 [longest](Ports const& b) { return b.size() == longest; });
    return getStrongestStrength(long_bridges);
}
