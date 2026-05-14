#include "Simulator.h"

#include "LogWriter.h"

#include <queue>

#include <algorithm>
#include <cassert>
#include <deque>
#include <optional>
#include <ostream>
#include <vector>

namespace workshop {

namespace {

struct Item {
  int id;
  int type;
};

using TimeTable = std::vector<std::vector<int>>;

struct Machine {
  explicit Machine(std::size_t idx) noexcept
      : index(idx) {}

  [[nodiscard]] bool isIdle() const noexcept {
    return !current.has_value();
  }

  std::size_t enqueue(Item item, TimeTable const& T) {
    std::size_t pos = queue.size();
    waitTimeSum += T[item.type][index];
    queue.push_back(item);
    return pos;
  }

  Item dequeueAndStart(TimeTable const& T) {
    Item item = queue.front();
    queue.pop_front();
    waitTimeSum -= T[item.type][index];
    current = item;
    return item;
  }

  Item completeProcessing() noexcept {
    Item item = *current;
    current = std::nullopt;
    ++item.type;
    return item;
  }

  std::size_t index;
  std::deque<Item> queue;
  long long waitTimeSum = 0;
  std::optional<Item> current;
};

enum class EventKind : int {
  Finish = 0,
  Start = 1,
  Wait = 2,
  Ready = 3,
  Stop = 4,
};

struct Event {
  long long time;
  EventKind kind;
  long long seq;
  int itemId = 0;
  int op = 0;
  std::size_t machine = 0;
  std::size_t queueLen = 0;
};

struct EventGreater {
  bool operator()(Event const& a, Event const& b) const noexcept {
    if (a.time != b.time) {
      return a.time > b.time;
    }
    if (a.kind != b.kind) {
      return a.kind > b.kind;
    }
    return a.seq > b.seq;
  }
};

} // namespace

void simulate(InputData const& input, std::ostream& out) {
  int M = input.M;
  TimeTable const& T = input.T;

  std::size_t S = 0;
  for (auto& q : input.queues) {
    S += q.size();
  }

  LogWriter log(out);

  if (S == 0) {
    log.writeLine("stop", 0LL);
    return;
  }

  std::vector<Machine> machines;
  machines.reserve(input.queues.size());
  for (std::size_t j = 0; j < input.queues.size(); ++j) {
    machines.emplace_back(j);
  }

  int nextId = 0;
  for (std::size_t j = 0; j < machines.size(); ++j) {
    for (int type : input.queues[j]) {
      machines[j].queue.push_back(Item{nextId++, type});
      machines[j].waitTimeSum += T[type][j];
    }
  }

  using EventQueue = std::priority_queue<Event, std::vector<Event>, EventGreater>;
  EventQueue events;

  long long seqCounter = 0;

  auto scheduleStart = [&](std::size_t j, long long t, Item const& item) {
    assert(item.type < M - 1);
    events.push(Event{t, EventKind::Start, seqCounter++, item.id, item.type, j});
    events.push(Event{t + T[item.type][j], EventKind::Finish, seqCounter++, item.id, item.type, j});
  };

  for (std::size_t j = 0; j < machines.size(); ++j) {
    if (!machines[j].queue.empty()) {
      scheduleStart(j, 0, machines[j].dequeueAndStart(T));
    }
  }

  std::size_t readyCount = 0;
  bool running = true;

  while (running) {
    Event ev = events.top();
    events.pop();

    switch (ev.kind) {
    case EventKind::Finish: {
      log.writeLine("finish", ev.time, ev.itemId, ev.op, ev.machine);

      Machine& m = machines[ev.machine];
      Item item = m.completeProcessing();

      if (item.type == M - 1) {
        events.push(Event{ev.time, EventKind::Ready, seqCounter++, item.id, -1, ev.machine});
      } else {
        auto minWaitIt = std::ranges::min_element(machines, std::less<>{}, &Machine::waitTimeSum);
        std::size_t best = static_cast<std::size_t>(std::ranges::distance(machines.begin(), minWaitIt));

        Machine& target = machines[best];
        bool const targetWasIdle = target.isIdle();
        std::size_t const pos = target.enqueue(item, T);
        events.push(Event{ev.time, EventKind::Wait, seqCounter++, item.id, item.type, best, pos});
        if (targetWasIdle) {
          Item toRun = target.dequeueAndStart(T);
          scheduleStart(best, ev.time, toRun);
        }
      }

      if (m.isIdle() && !m.queue.empty()) {
        scheduleStart(ev.machine, ev.time, m.dequeueAndStart(T));
      }
      break;
    }

    case EventKind::Start:
      log.writeLine("start", ev.time, ev.itemId, ev.op, ev.machine);
      break;

    case EventKind::Wait:
      log.writeLine("wait", ev.time, ev.itemId, ev.op, ev.machine, ev.queueLen);
      break;

    case EventKind::Ready:
      log.writeLine("ready", ev.time, ev.itemId, ev.machine);
      if (++readyCount == S) {
        events.push(Event{ev.time, EventKind::Stop, seqCounter++});
      }
      break;

    case EventKind::Stop:
      log.writeLine("stop", ev.time);
      running = false;
      break;
    }
  }
}

} // namespace workshop
