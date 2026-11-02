# Benchmark comparison scripts

Run the following commands from the repository root. See the
[benchmark documentation](../../benchmarks/README.md) for building the executable
and configuring datasets and mesh types.

## Compare branches

[compare_branches.py](compare_branches.py) builds the baseline branch in a
dedicated worktree and the current working tree, runs both, saves JSON reports
under `scripts/benchmarks/results/`, and compares them:

```sh
python scripts/benchmarks/compare_branches.py main \
    --mesh-type-a trimesh-indexed --mesh-type-b trimesh --grid 1000 1000
```

Side A is the baseline ref; side B is the current working tree.
`--mesh-type-a` and `--mesh-type-b` select each executable's **native** type.
`--mesh-type` is a shorthand for requesting the same name on both sides, not
an automatic equivalence mapping. The name `trimesh` means different reference
representations in v1 and v2.

With `--config config.json`, use `mesh_type_a`, `mesh_type_b`, and/or a common
`mesh_type`:

```json
{
    "mesh_type_a": "trimesh-indexed",
    "mesh_type_b": "trimesh",
    "grid": [1000, 1000],
    "include": ["stat/*"],
    "iterations": 10
}
```

For each side the precedence is:

1. Side-specific CLI option.
2. Common CLI `--mesh-type` (including forwarded `--args` in `compare.py`).
3. Side-specific configuration key.
4. Common configuration `mesh_type`.
5. The executable's native default when no type was requested.

Benchmark filters, exclusions, and dataset options are forwarded as well.
Use `--help` for all options.

When a type is explicitly requested through CLI or configuration, both reports
must confirm it in `metadata.mesh_type`. An older executable that ignores the
option or omits this metadata is rejected rather than silently benchmarking the
wrong representation.

### Recommended v1/v2 comparisons

| Baseline A (v1) | Comparison B (v2) | What is compared |
|-----------------|-------------------|------------------|
| `trimesh-indexed` | `trimesh` | double, 32-bit indices |
| `trimesh-indexedf` | `trimeshf` | float, 32-bit indices |
| `trimesh` | `trimesh64` | double, pointers vs 64-bit indices |
| `trimeshf` | `trimesh64f` | float, pointers vs 64-bit indices |

The last two pairs compare references of equal width **on 64-bit pointer
platforms**; pointers and indices are not equivalent representations and do not
imply identical mesh layouts or total memory use.
Names are never rewritten based on a branch name, so these commands also work
with tags, commit hashes, and v2/v2 comparisons.

### Preserving a v1 baseline

Before merging v2 into `main`, first ensure `main` has the final v1 benchmark
harness. Create an immutable baseline tag and a maintenance branch at that
commit (no separate GitHub fork is required):

```sh
git tag mesh-v1-baseline main
git branch mesh_v1 main
```

After the merge, run the script from the new `main`, using `mesh_v1` or the tag
as the baseline ref. Keep harness updates separate from algorithm changes so
they can be backported to `mesh_v1` without altering the baseline implementation.
Older v1 reports may omit the new representation metadata; requested native
types are still checked against `metadata.mesh_type`.

### Reproducibility and coverage

Branch comparisons save a `provenance` object in each JSON report: source commit,
dirty status, binary SHA-256, CMake/VCLib build cache entries, effective dataset
and mesh-type arguments, config contents, iteration override, and requested CPU.
When using `--no-build`, a warning explicitly states that the recorded source
commit/cache cannot prove which sources built the reused binary.

Use the same compiler, build flags, dataset, enabled components and measured
operation on both sides. The comparison table shows benchmarks present on only
one side, warns about differing coverage, and computes the geometric mean over
shared measurable benchmarks only. Matching names alone do not establish
equivalent workloads during the port.

## Compare binaries or reports

[compare.py](compare.py) can run two existing binaries:

```sh
python scripts/benchmarks/compare.py --bin-a ./baseline --bin-b ./current \
    --mesh-type-a trimesh-indexed --mesh-type-b trimesh \
    --args="--grid 1000 1000" --iterations 10
```

Side-specific types are checked separately against the two reports.
`--mesh-type` is forwarded to both binaries and checked against both reports.
Passing it inside `--args` is also supported and verified.
When `--args` contains `--config`, its `mesh_type` is verified too, unless
overridden by a CLI mesh type.

Alternatively, compare saved reports:

```sh
python scripts/benchmarks/compare.py baseline.json current.json \
    --label-a baseline --label-b current
```

Comparisons without an explicit mesh type remain compatible with older reports.
When the reports have different metadata, including different mesh types, both
sets of metadata are displayed.

## Regression tests

After building the benchmark executable:

```sh
python -m unittest discover -s scripts/benchmarks -p 'test_*.py'
```

The tests cover all four v2 mesh types on generated grids and an OBJ mesh,
registry consistency, CLI/config precedence, independent A/B selection,
invalid options, metadata, provenance, and coverage warnings.
CLI tests use the executable built by the benchmark preset; set
`VCLIB_MESH_BENCHMARK` to use another path. CLI tests are skipped if the executable
does not exist.
