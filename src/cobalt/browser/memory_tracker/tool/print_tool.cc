// Copyright 2017 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "cobalt/browser/memory_tracker/tool/print_tool.h"

#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/threading/platform_thread.h"
#include "cobalt/base/c_val.h"
#include "cobalt/browser/memory_tracker/tool/util.h"
#include "nb/analytics/memory_tracker.h"
#include "nb/analytics/memory_tracker_helpers.h"
#include "starboard/log.h"
#include "starboard/types.h"

namespace cobalt {
namespace browser {
namespace memory_tracker {

using nb::analytics::AllocationGroup;

class PrintTool::CvalsMap {
 public:
  typedef base::CVal<base::cval::SizeInBytes> CValType;

  ~CvalsMap() {
    while (!map_.empty()) {
      MapCvals::iterator it = map_.begin();
      delete it->second;
      map_.erase(it);
    }
  }

  void Update(const std::string& name, size_t value) {
    std::string cval_name = GetCvalName(name);
    CValType*& val = map_[cval_name];
    if (!val) {
      // Create if not found.
      val = new CValType(cval_name, 0,
                         "Automatically generated by the "
                         "browser::memory_tracker system.");
    }
    (*val) = value;
  }

  static std::string GetCvalName(const std::string& name) {
    std::stringstream ss;
    ss << "Memory.Scope." << name;
    return ss.str();
  }

 private:
  typedef std::map<std::string, CValType*> MapCvals;
  MapCvals map_;
};

PrintTool::PrintTool() : cvals_map_(new CvalsMap) {}

PrintTool::~PrintTool() {}

void PrintTool::Run(Params* params) {
  const std::string kSeperator =
      "--------------------------------------------------";

  while (!params->finished()) {
    std::vector<const AllocationGroup*> vector_output;
    params->memory_tracker()->GetAllocationGroups(&vector_output);

    typedef std::map<std::string, const AllocationGroup*> Map;
    typedef Map::const_iterator MapIt;

    Map output;
    for (size_t i = 0; i < vector_output.size(); ++i) {
      const AllocationGroup* group = vector_output[i];
      output[group->name()] = group;
    }

    int32 num_allocs = 0;
    int64 total_bytes = 0;

    struct F {
      static void PrintRow(std::stringstream* ss, const std::string& v1,
                           const std::string& v2, const std::string& v3) {
        ss->width(25);
        *ss << std::left << v1;
        ss->width(13);
        *ss << std::right << v2 << "  ";
        ss->width(10);
        *ss << std::right << v3 << "\n";
      }
    };

    if (params->memory_tracker()->IsMemoryTrackingEnabled()) {
      // If this isn't true then it would cause an infinite loop. The
      // following will likely crash.
      SB_DCHECK(false) << "Unexpected, memory tracking should be disabled.";
    }

    std::stringstream ss;

    ss << kNewLine;
    ss << "TimeNow " << params->TimeInMinutesString()
       << " (minutes):" << kNewLine << kNewLine;

    ss << kSeperator << kNewLine;
    nb::analytics::MemoryStats memstats =
        nb::analytics::GetProcessMemoryStats();

    F::PrintRow(&ss, "MALLOC STAT", "IN USE BYTES", "");
    ss << kSeperator << kNewLine;
    F::PrintRow(&ss, "Total CPU Reserved",
                NumberFormatWithCommas(memstats.total_cpu_memory), "");

    F::PrintRow(&ss, "Total CPU Used",
                NumberFormatWithCommas(memstats.used_cpu_memory), "");

    F::PrintRow(&ss, "Total GPU Reserved",
                NumberFormatWithCommas(memstats.total_gpu_memory), "");

    F::PrintRow(&ss, "Total GPU Used",
                NumberFormatWithCommas(memstats.used_gpu_memory), "");

    ss << kSeperator << kNewLine << kNewLine;

    ss << kSeperator << kNewLine;
    F::PrintRow(&ss, "MEMORY REGION", "IN USE BYTES", "NUM ALLOCS");
    ss << kSeperator << kNewLine;

    for (MapIt it = output.begin(); it != output.end(); ++it) {
      const AllocationGroup* group = it->second;
      if (!group) {
        continue;
      }

      int32 num_group_allocs = -1;
      int64 total_group_bytes = -1;

      group->GetAggregateStats(&num_group_allocs, &total_group_bytes);
      SB_DCHECK(-1 != num_group_allocs);
      SB_DCHECK(-1 != total_group_bytes);

      cvals_map_->Update(group->name(), static_cast<size_t>(total_group_bytes));

      num_allocs += num_group_allocs;
      total_bytes += total_group_bytes;

      F::PrintRow(&ss, it->first, NumberFormatWithCommas(total_group_bytes),
                  NumberFormatWithCommas(num_group_allocs));
    }

    cvals_map_->Update("Total", static_cast<size_t>(total_bytes));

    ss << kNewLine;

    F::PrintRow(&ss, "Total (in groups above)",
                NumberFormatWithCommas(total_bytes),
                NumberFormatWithCommas(num_allocs));

    ss << kSeperator << kNewLine;
    ss << kNewLine << kNewLine;

    params->logger()->Output(ss.str().c_str());
    // Output once every 5 seconds.
    base::PlatformThread::Sleep(base::TimeDelta::FromSeconds(5));
  }
}

std::string PrintTool::tool_name() const  {
  return "MemoryTrackerPrintThread";
}

}  // namespace memory_tracker
}  // namespace browser
}  // namespace cobalt
