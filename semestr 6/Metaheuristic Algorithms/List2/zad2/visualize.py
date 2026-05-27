"""
Reads tour CSV files from results/ and saves PNG plots.

Tour files (solution_*.csv or *_best_tour.csv):
    order,city_id,x,y

Trial files (*_trials.csv):
    trial,cost,time_ms
"""

import os
import glob
import math
import pandas as pd
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

os.chdir(os.path.dirname(os.path.abspath(__file__)))

def tour_length(df):
    xs, ys = df["x"].tolist(), df["y"].tolist()
    n = len(xs)
    return sum(math.hypot(xs[i]-xs[(i+1)%n], ys[i]-ys[(i+1)%n]) for i in range(n))


def plot_tour(csv_path):
    df = pd.read_csv(csv_path)
    xs = df["x"].tolist() + [df["x"].iloc[0]]
    ys = df["y"].tolist() + [df["y"].iloc[0]]

    fig, ax = plt.subplots(figsize=(10, 8))
    ax.plot(xs, ys, "b-", lw=0.7, alpha=0.6)
    ax.scatter(df["x"], df["y"], c="red", s=10, zorder=5)

    name = os.path.basename(csv_path).replace(".csv", "")
    cost = tour_length(df)
    ax.set_title(f"{name}  |  {len(df)} cities  |  cost = {cost:.0f}")
    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_aspect("equal", adjustable="datalim")
    plt.tight_layout()

    out = csv_path.replace(".csv", ".png")
    plt.savefig(out, dpi=150)
    plt.close()
    print(f"Saved {out}")


def plot_trials(csv_path):
    df = pd.read_csv(csv_path)
    fig, ax = plt.subplots(figsize=(10, 5))
    ax.plot(df["trial"], df["cost"], "b-o", ms=3, lw=0.8)
    ax.axhline(df["cost"].min(),  color="r", ls="--", lw=1.2,
               label=f"best  = {df['cost'].min():.0f}")
    ax.axhline(df["cost"].mean(), color="g", ls="--", lw=1.2,
               label=f"avg = {df['cost'].mean():.0f}")
    ax.set_xlabel("Trial")
    ax.set_ylabel("Tour cost")
    name = os.path.basename(csv_path).replace("_trials.csv", "")
    ax.set_title(f"{name}  |  {len(df)} trials")
    ax.legend()
    plt.tight_layout()

    out = csv_path.replace(".csv", ".png")
    plt.savefig(out, dpi=150)
    plt.close()
    print(f"Saved {out}")


for f in glob.glob("results\\solution_*.csv") + glob.glob("results\\*_best_tour.csv"):
    plot_tour(f)

for f in glob.glob("results\\*_trials.csv"):
    plot_trials(f)
