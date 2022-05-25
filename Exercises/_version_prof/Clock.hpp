///
/// Exemples des slides avec des horloges sur l'héritage, le polymorphisme et la conversion.
///

#pragma once


#include <cstddef>
#include <cstdint>

#include <string>
#include <ctime>
#include <iostream>
#include <concepts>

#include <cppitertools/itertools.hpp>

using namespace std;
using namespace iter;


class Clock {
public:
	Clock(bool useMilitary)
	: isMilitary_(useMilitary) {
		update();
	}

	virtual ~Clock() = default;

	virtual string getLocation() const {
		return "Local";
	}

	bool isMilitary() const {
		return isMilitary_;
	}

	virtual int getHours() const {
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
		snprintf(hm, sizeof(hm) / sizeof(hm[0]), "%2i:%02i", clock.getHours(), clock.getMinutes());
		return out << clock.getLocation() << ": " << hm;
	}

private:
	bool isMilitary_ = false;
	tm   now_ = {};
};


class TravelClock : public Clock {
public:
	TravelClock (bool useMilitary, const string& location, int timeDiff)
	: Clock(useMilitary),
	  location_(location),
	  timeDiff_(timeDiff) { }

	~TravelClock() = default;

	string getLocation() const override {
		return location_;
	}

	int getHours() const override {
		static auto posMod = [] (int x, int n) {
			return (x % n) + (x < 0 ? n : 0);
		};
		int h = Clock::getHours() + timeDiff_;
		return posMod(h, isMilitary() ? 24 : 12);
	}

	int getTimeDiff() const {
		return timeDiff_;
	}

private:
	string location_;
	int    timeDiff_;
};

