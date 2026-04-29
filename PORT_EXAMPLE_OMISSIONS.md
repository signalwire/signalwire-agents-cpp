# PORT_EXAMPLE_OMISSIONS — examples in Python that the C++ port deliberately does NOT mirror

`audit_example_parity.py` walks the Python SDK's `examples/` directory and
checks that every Python example has a C++-equivalent under the C++ port's
`examples/` directory (file-stem match after lowercase + alnum normalize).
Files listed here are explicit deviations.

Format: `- <python_example_filename>: <one-sentence rationale>`

The skip list is small. Search-related examples (the Python SDK's
`signalwire/search/` subsystem — pgvector / sigmond / vector indexing) are
out of scope per the porting-sdk parity rule (search lives only in Python).
Bedrock examples are deferred until the port adopts Amazon Bedrock per the
porting-sdk's per-port capability decision (see CHECKLIST.md Phase 9).

## Search-related (Python-only feature)

- local_search_agent.py: search subsystem (pgvector + transformer model) is
  Python-only per the porting-sdk skip list; the example demonstrates that
  feature exclusively.
