///
/// Exemples des slides avec des horloges sur l'héritage, le polymorphisme et la conversion.
///

#pragma once


#include <cstddef>
#include <cstdint>

#include <string>
#include <ctime>
#include <iostream>

#include <cppitertools/itertools.hpp>

using namespace std;
using namespace iter;


class Clock {
public:
	Clock(bool useMilitary)
	: isMilitary_(useMilitary) {
		update();
	}

	~Clock() = default;

	string getLocation() const {
		return "Local";
	}

	bool isMilitary() const {
		return isMilitary_;
	}

	int getHours() const {
		return now_.tm_hour % (isMilitary_ ? 24 : 12);
	}

	int getMinutes() const {
		return now_.tm_min;
	}

	void update() {
		now_.tm_hour = 14;
		now_.tm_min = 9;
	}

	friend ostream& operator<<(ostream& out, const Clock& clock) {
		char hm[32] = "";
		snprintf(hm, 32, "%2i:%02i", clock.getHours(), clock.getMinutes());
		return out << clock.getLocation() << ": " << hm;
	}

private:
	bool isMilitary_;
	tm   now_ = {};
};


class TravelClock : public Clock {
public:
	TravelClock (bool useMilitary, const string& location, int timeDiff)
	: Clock(useMilitary),
	  location_(location),
	  timeDiff_(timeDiff) { }

	string getLocation() const {
		return location_;
	}

	int getHours() const {
		static auto posMod = [] (int x, int n) {
			return (x % n) + (x < 0 ? n : 0);
		};
		int h = Clock::getHours() + timeDiff_;
		return posMod(h, isMilitary() ? 24 : 12);
	}

private:
	string location_;
	int    timeDiff_;
};

