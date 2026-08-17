---
layout: course
title: "Weekly Examples"
permalink: /agent-sandbox-10weeks/code/WEEKLY_EXAMPLES.html
---

# Danh sách ví dụ theo tuần

## Tuần 01 — risk_model.cpp / risk_model.py
Phân loại rủi ro theo năng lực tool.
`g++ -std=c++17 -O2 week01/risk_model.cpp -o r && ./r` · `python week01/risk_model.py`

## Tuần 02 — isolation_levels.cpp / isolation_levels.py
So sánh mức cô lập + syscall allowlist.
`g++ -std=c++17 -O2 week02/isolation_levels.cpp -o r && ./r`

## Tuần 03 — pipeline.cpp / architecture_sim.py
Mô phỏng pipeline control/data plane.
`g++ -std=c++17 -O2 week03/pipeline.cpp -o r && ./r`

## Tuần 04 — snapshot.cpp / snapshot_sim.py
Snapshot/restore bộ nhớ + dirty-page.
`g++ -std=c++17 -O2 week04/snapshot.cpp -o r && ./r`

## Tuần 05 — cow.cpp / cow_storage.py
Copy-on-Write (refcount + tách khi ghi).
`g++ -std=c++17 -O2 week05/cow.cpp -o r && ./r`

## Tuần 06 — policy.cpp / network_policy.py
Network policy CIDR longest-prefix-match.
`g++ -std=c++17 -O2 week06/policy.cpp -o r && ./r`

## Tuần 07 — egress.cpp / egress_filter.py
Egress zero-trust: allowlist + injection.
`g++ -std=c++17 -O2 week07/egress.cpp -o r && ./r`

## Tuần 08 — tool_runner.cpp / tool_runner.py
Wrapper chạy tool an toàn (whitelist/denylist).
`g++ -std=c++17 -O2 week08/tool_runner.cpp -o r && ./r --cmd "rm -rf /"`

## Tuần 09 — lifecycle.cpp / lifecycle_sim.py
State machine vòng đời + auto-pause.
`g++ -std=c++17 -O2 week09/lifecycle.cpp -o r && ./r`

## Tuần 10 — harden_check.cpp / harden_check.py
Checklist hardening + syscall allowlist.
`g++ -std=c++17 -O2 week10/harden_check.cpp -o r && ./r --passed 1,2,3`
