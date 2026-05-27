import os
import glob
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import seaborn as sns
import numpy as np

RESULTS_DIR = "results"
PLOTS_DIR = os.path.join(RESULTS_DIR, "plots")
os.makedirs(PLOTS_DIR, exist_ok=True)

sns.set_theme(style="whitegrid", palette="tab10")

# ─── Load summary ─────────────────────────────────────────────────────────────

summary_path = os.path.join(RESULTS_DIR, "summary.csv")
if not os.path.exists(summary_path):
    print(f"No summary.csv found in {RESULTS_DIR}/")
    exit(1)

df = pd.read_csv(summary_path)
print(f"Loaded {len(df)} rows from summary.csv")
print(df.head())

# Create a short label combining algorithm + crossover + flags
def make_label(row):
    label = row["algorithm"] + "_" + row["crossover"]
    return label

df["label"] = df.apply(make_label, axis=1)

instances = sorted(df["instance"].unique())
labels = sorted(df["label"].unique())

# ─── 1. Convergence plots ─────────────────────────────────────────────────────

conv_files = glob.glob(os.path.join(RESULTS_DIR, "*_conv.csv"))

if conv_files:
    # Group by instance
    instance_conv = {}
    for path in conv_files:
        basename = os.path.basename(path)
        # filename: <instance>_<algo>_<xover>_conv.csv
        parts = basename.replace("_conv.csv", "").split("_")
        # instance name may contain numbers, algo labels have known names
        # heuristic: rejoin and split on known keywords
        known_algos = ["GA", "Memetic", "Island", "Parallel"]
        label_start = -1
        for i, p in enumerate(parts):
            if p in known_algos:
                label_start = i
                break
        if label_start == -1:
            continue
        inst_name = "_".join(parts[:label_start])
        algo_label = "_".join(parts[label_start:])

        try:
            cdf = pd.read_csv(path)
            if inst_name not in instance_conv:
                instance_conv[inst_name] = []
            instance_conv[inst_name].append((algo_label, cdf))
        except Exception as e:
            print(f"  Could not read {path}: {e}")

    for inst, entries in instance_conv.items():
        fig, ax = plt.subplots(figsize=(10, 5))
        for algo_label, cdf in entries:
            ax.plot(cdf["generation"], cdf["best_fitness"], label=f"{algo_label} best")
            ax.plot(cdf["generation"], cdf["avg_fitness"], linestyle="--",
                    alpha=0.6, label=f"{algo_label} avg")
        ax.set_title(f"Convergence — {inst}")
        ax.set_xlabel("Generation")
        ax.set_ylabel("Tour length")
        ax.legend(fontsize=8)
        ax.yaxis.set_major_formatter(ticker.FuncFormatter(lambda x, _: f"{x:,.0f}"))
        plt.tight_layout()
        out = os.path.join(PLOTS_DIR, f"convergence_{inst}.png")
        plt.savefig(out, dpi=150)
        plt.close()
        print(f"Saved {out}")
else:
    print("No convergence files found, skipping convergence plots.")

# ─── 2. Box plots: crossover comparison per instance ─────────────────────────

xover_labels = df[df["island"] == False]["label"].unique()
if len(xover_labels) >= 2:
    for inst in instances:
        sub = df[(df["instance"] == inst) & (df["island"] == False)]
        if sub.empty:
            continue
        fig, ax = plt.subplots(figsize=(max(6, len(xover_labels) * 2), 5))
        order = sorted(sub["label"].unique())
        sns.boxplot(data=sub, x="label", y="best_tour", order=order, ax=ax)
        ax.set_title(f"Crossover comparison — {inst} (n={sub['n'].iloc[0]})")
        ax.set_xlabel("Algorithm")
        ax.set_ylabel("Tour length")
        ax.yaxis.set_major_formatter(ticker.FuncFormatter(lambda x, _: f"{x:,.0f}"))
        plt.tight_layout()
        out = os.path.join(PLOTS_DIR, f"boxplot_{inst}.png")
        plt.savefig(out, dpi=150)
        plt.close()
        print(f"Saved {out}")

# ─── 3. Bar chart: avg best tour per algorithm per instance ───────────────────

agg = df.groupby(["instance", "n", "label"])["best_tour"].agg(["mean", "std", "min"]).reset_index()
agg.columns = ["instance", "n", "label", "mean_tour", "std_tour", "min_tour"]
agg = agg.sort_values(["n", "instance"])

fig, axes = plt.subplots(
    nrows=max(1, len(instances) // 3 + (1 if len(instances) % 3 else 0)),
    ncols=min(3, len(instances)),
    figsize=(6 * min(3, len(instances)), 4 * max(1, len(instances) // 3 + 1)),
    squeeze=False
)
axes_flat = [ax for row in axes for ax in row]

for idx, inst in enumerate(sorted(instances, key=lambda i: df[df["instance"] == i]["n"].iloc[0])):
    ax = axes_flat[idx]
    sub = agg[agg["instance"] == inst]
    x = range(len(sub))
    bars = ax.bar(x, sub["mean_tour"], yerr=sub["std_tour"], capsize=4,
                  color=sns.color_palette("tab10", len(sub)))
    ax.set_xticks(list(x))
    ax.set_xticklabels(sub["label"].tolist(), rotation=30, ha="right", fontsize=8)
    ax.set_title(f"{inst} (n={sub['n'].iloc[0]})", fontsize=9)
    ax.set_ylabel("Avg tour length", fontsize=8)
    ax.yaxis.set_major_formatter(ticker.FuncFormatter(lambda x, _: f"{x:,.0f}"))

for idx in range(len(instances), len(axes_flat)):
    axes_flat[idx].set_visible(False)

plt.suptitle("Average Best Tour Length by Algorithm", fontsize=12)
plt.tight_layout()
out = os.path.join(PLOTS_DIR, "comparison_bar.png")
plt.savefig(out, dpi=150)
plt.close()
print(f"Saved {out}")

# ─── 4. Summary table as PNG ──────────────────────────────────────────────────

pivot = agg.pivot(index="instance", columns="label", values="mean_tour")
pivot = pivot.sort_values(
    by=list(pivot.columns)[0] if len(pivot.columns) > 0 else pivot.columns[0]
)

fig, ax = plt.subplots(figsize=(max(8, len(pivot.columns) * 2.5), max(4, len(pivot) * 0.6 + 2)))
ax.axis("off")

cell_text = []
for _, row in pivot.iterrows():
    cell_text.append([f"{v:,.1f}" if not np.isnan(v) else "—" for v in row])

table = ax.table(
    cellText=cell_text,
    rowLabels=pivot.index.tolist(),
    colLabels=pivot.columns.tolist(),
    cellLoc="center",
    loc="center"
)
table.auto_set_font_size(False)
table.set_fontsize(9)
table.scale(1.2, 1.5)

ax.set_title("Summary: Average Best Tour Length", fontsize=12, pad=20)
plt.tight_layout()
out = os.path.join(PLOTS_DIR, "summary_table.png")
plt.savefig(out, dpi=150, bbox_inches="tight")
plt.close()
print(f"Saved {out}")

# ─── 5. Runtime comparison ────────────────────────────────────────────────────

if "time_ms" in df.columns:
    time_agg = df.groupby(["instance", "n", "label"])["time_ms"].mean().reset_index()
    time_agg = time_agg.sort_values("n")

    fig, ax = plt.subplots(figsize=(12, 5))
    for lbl in sorted(df["label"].unique()):
        sub = time_agg[time_agg["label"] == lbl].sort_values("n")
        if not sub.empty:
            ax.plot(sub["instance"] + "\n(n=" + sub["n"].astype(str) + ")",
                    sub["time_ms"] / 1000, marker="o", label=lbl)
    ax.set_title("Average Runtime per Instance and Algorithm")
    ax.set_xlabel("Instance")
    ax.set_ylabel("Time (s)")
    ax.legend()
    plt.xticks(rotation=30, ha="right", fontsize=8)
    plt.tight_layout()
    out = os.path.join(PLOTS_DIR, "runtime.png")
    plt.savefig(out, dpi=150)
    plt.close()
    print(f"Saved {out}")

print(f"\nAll plots saved to {PLOTS_DIR}/")
