#include "otpch.h"

#include "configmanager.h"
#include "otserv.h"
#include "tools.h"

#include "map.h"
#include <chrono>
#include <iostream>
#include <vector>
#include <numeric> // For std::accumulate
#include <algorithm> // For std::sort

// Forward declaration of AStarNodes and AStarNode_ptr if not in a common header
// For demonstration, assume AStarNode and AStarNode_ptr are defined elsewhere
// and AStarNodes has a public constructor and methods as in your code.

void benchmarkAStarNodesCreation(int numCreations)
{
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < numCreations; ++i) {
		AStarNodes nodes(static_cast<uint16_t>(i % 100), static_cast<uint16_t>((i / 100) % 100));
		// Potentially add more operations here if needed for a realistic scenario
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> duration = end - start;
	std::clog << "AStarNodes Creation Benchmark (" << numCreations << " ops): " << duration.count() << " ms\n";
}

void benchmarkGetBestNode(int numNodes, int numGets)
{
	if (numNodes == 0) {
		std::clog << "Cannot benchmark getBestNode with zero initial nodes.\n";
		return;
	}
	AStarNodes nodes(0, 0);              // Initial node
	for (int i = 1; i < numNodes; ++i) { // Add more nodes
		// Simplified: creating nodes directly without a parent or realistic g/f values for this example
		nodes.createNewNode(nullptr, static_cast<uint16_t>(i), static_cast<uint16_t>(i), static_cast<uint16_t>(i * 10),
		                    static_cast<uint16_t>(i * 20));
	}

	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < numGets; ++i) {
		if (nodes.nodesEmpty()) break; // Stop if no more nodes
		nodes.getBestNode();
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> duration = end - start;
	std::clog << "AStarNodes getBestNode Benchmark (" << numGets << " gets from " << numNodes
	          << " initial nodes): " << duration.count() << " ms\n";
}

static bool argumentsHandler(const std::vector<std::string_view>& args)
{
	for (const auto& arg : args) {
		if (arg == "--help") {
			std::clog << "Usage:\n"
			             "\n"
			             "\t--config=$1\t\tAlternate configuration file path.\n"
			             "\t--ip=$1\t\t\tIP address of the server.\n"
			             "\t\t\t\tShould be equal to the global IP.\n"
			             "\t--login-port=$1\tPort for login server to listen on.\n"
			             "\t--game-port=$1\tPort for game server to listen on.\n";
			return false;
		} else if (arg == "--version") {
			printServerVersion();
			return false;
		}

		auto tmp = explodeString(arg, "=");
		
		
		if (tmp[0] == "--config")
			ConfigManager::setString(ConfigManager::CONFIG_FILE, tmp[1]);
		else if (tmp[0] == "--ip")
			ConfigManager::setString(ConfigManager::IP, tmp[1]);
		else if (tmp[0] == "--login-port")
			ConfigManager::setNumber(ConfigManager::LOGIN_PORT, std::stoi(tmp[1].data()));
		else if (tmp[0] == "--game-port")
			ConfigManager::setNumber(ConfigManager::GAME_PORT, std::stoi(tmp[1].data()));
	}

	return true;
}

int main(int argc, const char** argv)
{
	std::vector<std::string_view> args(argv, argv + argc);
	for (const auto& arg : args) {
		std::clog << arg.data() << "\n";
		auto tmp = explodeString(arg, "=");
		if (tmp[0] == "--bestnodebench") {
			auto tmp2 = explodeString(tmp[1], ":");

			int numNodes = tmp2.size() >= 1 ? std ::stoi(tmp2[0].data()) : 1000;
			int numGets = tmp2.size() >= 2 ? std ::stoi(tmp2[1].data()) : 500;
			benchmarkGetBestNode(numNodes, numGets);
			return 1;
		} else if (tmp[0] == "--astarnodebench") {
			auto tmp2 = explodeString(tmp[1], ":");
			int numCreations = tmp2.size() >= 1 ? std::stoi(tmp2[0].data()) : 10000;
			benchmarkAStarNodesCreation(numCreations);
			return 1;
		}
	}

	if (!argumentsHandler(args)) {
		return 1;
	}

	startServer();
	return 0;
}
