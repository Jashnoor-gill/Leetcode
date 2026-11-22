
import numpy as np
import matplotlib.pyplot as plt
import scipy.stats as stats
from scipy.special import beta as beta_func
from scipy.stats import beta, norm
from scipy.optimize import minimize


# ============================================================
# PROBLEM 1: BAYESIAN INFERENCE - BETA BINOMIAL
# ============================================================

# 1. DEFINE SCENARIOS: (n, y, a, b)

scenarios = [
    [(20, 7, 2, 2), (20, 7, 8, 2), (20, 7, 2, 8)],
    [(60, 26, 2, 5), (60, 26, 10, 3), (60, 26, 3, 12)],
    [(250, 100, 2, 3), (250, 100, 15, 5), (250, 100, 5, 20)]
]

# 2. DEFINE THETA GRID

theta = np.linspace(0.0001, 0.9999, 1000)


# 3. DISTRIBUTION FUNCTIONS

def prior_fn(theta, a, b):
    return (theta**(a-1) * (1-theta)**(b-1)) / beta_func(a, b)


def normalised_likelihood_fn(theta, n, y):
    raw_lik = theta**y * (1-theta)**(n-y)
    area = np.trapezoid(raw_lik, theta)
    return raw_lik / area


def posterior_fn(theta, n, y, a, b):
    a_post = a + y
    b_post = b + n - y

    return (
        theta**(a_post-1) *
        (1-theta)**(b_post-1)
    ) / beta_func(a_post, b_post)


# 4. COMPUTE AND SAVE CURVES FOR ALL 9 SCENARIOS

plot_data = []

for r in range(3):
    row_results = []

    for c in range(3):

        n, y, a, b = scenarios[r][c]

        prior_y = prior_fn(theta, a, b)
        lik_y = normalised_likelihood_fn(theta, n, y)
        post_y = posterior_fn(theta, n, y, a, b)

        row_results.append({
            "n": n,
            "y": y,
            "a": a,
            "b": b,
            "prior_y": prior_y,
            "lik_y": lik_y,
            "post_y": post_y
        })

    plot_data.append(row_results)


# 5. PLOT ALL 9 SCENARIOS

fig, axes = plt.subplots(
    nrows=3, ncols=3,
    figsize=(16, 11),
    sharex=True
)

plt.subplots_adjust(hspace=0.35, wspace=0.22)

for r in range(3):
    for c in range(3):

        cell = plot_data[r][c]

        n = cell["n"]
        y = cell["y"]
        a = cell["a"]
        b = cell["b"]

        ax = axes[r, c]

        ax.plot(
            theta, cell["prior_y"],
            color="#1b365d",
            linestyle="--",
            lw=1.5,
            label=f"Prior Beta({a}, {b})"
        )

        ax.plot(
            theta, cell["lik_y"],
            color="#2e7d32",
            linestyle=":",
            lw=1.8,
            label="Likelihood (normalised)"
        )

        ax.plot(
            theta, cell["post_y"],
            color="#c67117",
            linestyle="-",
            lw=1.2,
            label=f"Posterior Beta({a+y}, {b+n-y})"
        )

        ax.spines["top"].set_visible(False)
        ax.spines["right"].set_visible(False)

        ax.set_xlim(0, 1)
        ax.set_ylim(bottom=0)

        ax.set_title(
            f"n={n}, y={y}",
            fontsize=10.5,
            fontweight="bold"
        )

        ax.legend(
            loc="upper right",
            frameon=False,
            fontsize=8.5
        )

        if c == 0:
            ax.set_ylabel("Density", fontsize=10)

        if r == 2:
            ax.set_xlabel("Theta", fontsize=11)

fig.suptitle(
    "Prior, Normalised Likelihood, and Posterior Across (n, a, b)",
    fontsize=14,
    fontweight="bold",
    y=0.96
)

plt.show()


# 6. POSTERIOR SUMMARY FOR SELECTED SCENARIO

# n=60, y=26, a=2, b=5
# Posterior = Beta(28, 39)

a_post, b_post = 28, 39

# Mean
mean = a_post / (a_post + b_post)

# Median
median = beta.ppf(0.5, a_post, b_post)

# Mode
mode = (a_post - 1) / (a_post + b_post - 2)

# Standard deviation
sd = np.sqrt(
    (a_post * b_post) /
    ((a_post + b_post)**2 * (a_post + b_post + 1))
)


# 7. CREDIBLE INTERVALS AND TAIL PROBABILITY

# 95% Equal-tailed credible interval

ci_equal = beta.ppf(
    [0.025, 0.975],
    a_post,
    b_post
)

# 95% HPD credible interval

res = minimize(
    lambda p: (
        beta.ppf(p + 0.95, a_post, b_post)
        - beta.ppf(p, a_post, b_post)
    ),
    x0=0.025,
    bounds=[(0, 0.05)]
)

p_low = res.x[0]

ci_hpd = beta.ppf(
    [p_low, p_low + 0.95],
    a_post,
    b_post
)

# P(theta > 0.5)

prob_gt_05 = 1 - beta.cdf(
    0.5,
    a_post,
    b_post
)


# 8. SAVE AND PRINT SUMMARY

summary = {
    "a": a_post,
    "b": b_post,
    "mean": mean,
    "mode": mode,
    "ci": ci_equal,
    "prob": prob_gt_05
}

print(
    f"Beta({a_post}, {b_post}) | "
    f"Mean: {mean:.3f}, "
    f"Median: {median:.3f}, "
    f"Mode: {mode:.3f}, "
    f"SD: {sd:.3f}"
)

print(
    f"95% Equal CrI: [{ci_equal[0]:.3f}, {ci_equal[1]:.3f}] | "
    f"95% HPD CrI: [{ci_hpd[0]:.3f}, {ci_hpd[1]:.3f}]"
)

print(f"P(theta > 0.5 | y) = {prob_gt_05:.3f}")


# 9. POSTERIOR SUMMARY PLOT

theta = np.linspace(0.0001, 0.9999, 1000)

pdf = beta.pdf(
    theta,
    summary["a"],
    summary["b"]
)

fig, ax = plt.subplots(figsize=(8, 4.5))

ax.plot(theta, pdf, color="#c67117", lw=2)

lo, hi = summary["ci"]

# Shade 95% credible interval

ax.fill_between(
    theta[(theta >= lo) & (theta <= hi)],
    pdf[(theta >= lo) & (theta <= hi)],
    color="#f3d2b3",
    alpha=0.7
)

# Shade P(theta > 0.5)

ax.fill_between(
    theta[theta >= 0.5],
    pdf[theta >= 0.5],
    color="#c97a7e",
    alpha=0.65
)

# Mode line

ax.axvline(
    summary["mode"],
    color="#2e7d32",
    ls="--",
    lw=1.4
)

ax.text(
    summary["mode"] - 0.015,
    max(pdf)*0.95,
    f"Mode {summary['mode']:.3f}",
    color="#2e7d32",
    ha="right",
    fontweight="bold"
)

# Mean line

ax.axvline(
    summary["mean"],
    color="#1b365d",
    ls="--",
    lw=1.4
)

ax.text(
    summary["mean"] + 0.015,
    max(pdf)*1.02,
    f"Mean {summary['mean']:.3f}",
    color="#1b365d",
    ha="left",
    fontweight="bold"
)

ax.text(
    0.62,
    max(pdf)*0.72,
    f"P(theta > 0.5) = {summary['prob']:.3f}",
    color="#a94442"
)

ax.text(
    0.62,
    max(pdf)*0.52,
    f"95% CrI [{lo:.3f}, {hi:.3f}]",
    color="#8a6d3b"
)

ax.spines[["top", "right"]].set_visible(False)

ax.set(
    xlim=(0, 1),
    ylim=(0, max(pdf)*1.15),
    xlabel="Theta",
    ylabel="Density",
    title=f"Posterior Summary: Beta({summary['a']}, {summary['b']})"
)

plt.tight_layout()
plt.show()


# ============================================================
# PROBLEM 2: NORMAL-NORMAL BAYESIAN INFERENCE
# ============================================================

# 1. FIXED PARAMETERS

mu_0, tau_0 = 10.0, 2.0

sigma, y_bar = 3.0, 13.0

n_values = [5, 15, 30, 70]

mu_grid = np.linspace(2, 20, 1000)


# 2. POSTERIOR PARAMETER FUNCTION

def posterior_params_fn(n, mu_0, tau_0, sigma, y_bar):

    # Prior weight
    w_n = sigma**2 / (
        sigma**2 + n * tau_0**2
    )

    # Posterior variance
    tau_n_sq = 1.0 / (
        (1.0 / tau_0**2) +
        (n / sigma**2)
    )

    # Posterior mean
    mu_n = (
        w_n * mu_0 +
        (1.0 - w_n) * y_bar
    )

    return w_n, mu_n, tau_n_sq


# 3. NORMAL PDF FUNCTION

def normal_pdf_fn(x, mean, sd):

    return norm.pdf(
        x,
        loc=mean,
        scale=sd
    )


# 4. COMPUTE AND SAVE CURVES

plot_data = []

for n in n_values:

    w_n, mu_n, tau_n_sq = posterior_params_fn(
        n, mu_0, tau_0, sigma, y_bar
    )

    tau_n = np.sqrt(tau_n_sq)

    lik_sd = sigma / np.sqrt(n)

    plot_data.append({

        "n": n,
        "w_n": w_n,
        "mu_0": mu_0,
        "tau_0": tau_0,
        "y_bar": y_bar,

        "lik_var": sigma**2 / n,

        "mu_n": mu_n,
        "tau_n": tau_n,

        "prior_y": normal_pdf_fn(
            mu_grid, mu_0, tau_0
        ),

        "lik_y": normal_pdf_fn(
            mu_grid, y_bar, lik_sd
        ),

        "post_y": normal_pdf_fn(
            mu_grid, mu_n, tau_n
        )
    })

    print(
        f"n = {n:3d} | "
        f"w_n = {w_n:.3f} | "
        f"Data weight = {1-w_n:.3f} | "
        f"mu_n = {mu_n:.2f} (MLE = {y_bar})"
    )


# 5. PLOT ALL 4 SAMPLE SIZES

fig, axes = plt.subplots(
    2, 2,
    figsize=(14, 9.5),
    sharex=True
)

plt.subplots_adjust(hspace=0.35, wspace=0.20)

for idx, ax in enumerate(axes.flat):

    d = plot_data[idx]

    ax.plot(
        mu_grid,
        d["prior_y"],
        color="#1b365d",
        ls="--",
        lw=1.6,
        label=rf"Prior N({d['mu_0']:.0f}, {d['tau_0']:.0f}^2)"
    )

    ax.plot(
        mu_grid,
        d["lik_y"],
        color="#2e7d32",
        ls=":",
        lw=1.9,
        label=rf"Likelihood: y_bar={d['y_bar']:.0f}"
    )

    ax.plot(
        mu_grid,
        d["post_y"],
        color="#c67117",
        ls="-",
        lw=1.3,
        label=rf"Posterior N({d['mu_n']:.2f}, {d['tau_n']:.2f}^2)"
    )

    # Prior mean and MLE lines

    ax.axvline(
        d["mu_0"],
        color="#1b365d",
        ls=":",
        lw=1,
        alpha=0.6
    )

    ax.axvline(
        d["y_bar"],
        color="#2e7d32",
        ls=":",
        lw=1,
        alpha=0.6
    )

    ax.spines[["top", "right"]].set_visible(False)

    ax.set(
        xlim=(2, 20),
        ylim=(0, max(d["post_y"])*1.25),
        ylabel="Density",
        title=(
            f"n = {d['n']} | "
            f"w = {d['w_n']:.3f} | "
            f"mu_n = {d['mu_n']:.2f} "
            f"(MLE = {d['y_bar']:.0f})"
        )
    )

    ax.legend(
        loc="upper left",
        frameon=False,
        fontsize=8.5
    )

for ax in axes.flat:
    ax.set_xlabel("Mu", fontsize=11)

plt.show()


# ============================================================
# PROBLEM 3: METROPOLIS-HASTINGS ALGORITHM
# ============================================================

# 1. PARAMETERS AND STEP SIZES

np.random.seed(42)

n_iter = 20000

burn_in = 1000

theta_init = 3.0

step_sizes = [0.1, 0.5, 1, 2.4, 5, 25]


# 2. LOG TARGET DENSITY

def log_q_fn(theta):

    # Unnormalised log-density of N(0,1)

    return -0.5 * (theta**2)


# 3. METROPOLIS-HASTINGS SAMPLER

def metropolis_sampler(n_iter, theta_init, s):

    chain = np.zeros(n_iter)

    chain[0] = theta_init

    accepted = 0

    for t in range(n_iter - 1):

        theta_curr = chain[t]

        # Random walk proposal

        theta_prop = theta_curr + np.random.normal(
            0.0, s
        )

        # Log acceptance ratio

        log_diff = (
            log_q_fn(theta_prop) -
            log_q_fn(theta_curr)
        )

        # Acceptance condition

        if np.log(np.random.uniform(0.0, 1.0)) <= log_diff:

            chain[t+1] = theta_prop

            accepted += 1

        else:

            chain[t+1] = theta_curr

    accept_rate = accepted / (n_iter - 1)

    return chain, accept_rate


# 4. EFFECTIVE SAMPLE SIZE (ESS)

def compute_ess(samples):

    n = len(samples)

    centered = samples - np.mean(samples)

    var = np.var(samples)

    acf = np.correlate(
        centered,
        centered,
        mode="full"
    )[n-1:] / (n * var)

    tau = 1.0

    for k in range(1, n//2):

        if acf[k] <= 0:
            break

        tau += 2.0 * acf[k]

    return n / tau


# 5. RUN MH FOR ALL STEP SIZES

mh_results = []

print(
    f"{'Step size (s)':<14} | "
    f"{'Accept Rate':<12} | "
    f"{'ESS (19k draws)':<15}"
)

print("-" * 47)

for s in step_sizes:

    chain, acc = metropolis_sampler(
        n_iter,
        theta_init,
        s
    )

    post_draws = chain[burn_in:]

    ess = compute_ess(post_draws)

    mh_results.append({

        "s": s,

        "chain": chain,

        "post_draws": post_draws,

        "accept_rate": acc,

        "ess": ess
    })

    print(
        f"s = {s:<10} | "
        f"{acc:<12.3f} | "
        f"{ess:<15.0f}"
    )


# 6. PREPARE PLOT DATA

s_vals = [
    r["s"] for r in mh_results
]

acc_vals = [
    r["accept_rate"] for r in mh_results
]

ess_vals = [
    r["ess"] for r in mh_results
]

# Find best ESS

best_idx = int(np.argmax(ess_vals))

showcase_indices = [
    0,
    best_idx,
    len(mh_results)-1
]

showcase_colors = [
    "#1b365d",
    "#2e7d32",
    "#c67117"
]


# 7. PLOT ACCEPTANCE RATE, ESS AND TRACE PLOTS

fig = plt.figure(figsize=(15, 9))

gs = fig.add_gridspec(
    2, 3,
    height_ratios=[1.1, 1],
    hspace=0.42,
    wspace=0.25
)


# Acceptance Rate vs Step Size

ax_acc = fig.add_subplot(gs[0, :1])

ax_acc.plot(
    s_vals,
    acc_vals,
    marker="o",
    color="#1b365d",
    lw=2
)

ax_acc.axhline(
    0.44,
    color="gray",
    ls="--",
    lw=1.2,
    label="Optimal 1D rate (~0.44)"
)

ax_acc.set_xscale("log")

ax_acc.spines[["top", "right"]].set_visible(False)

ax_acc.set(
    xlabel="Step size (log scale)",
    ylabel="Acceptance Rate",
    ylim=(0, 1.05),
    title="Acceptance Rate vs Step Size"
)

ax_acc.legend(
    loc="upper right",
    frameon=False,
    fontsize=8.5
)


# ESS vs Step Size

ax_ess = fig.add_subplot(gs[0, 1:])

ax_ess.plot(
    s_vals,
    ess_vals,
    marker="o",
    color="#2e7d32",
    lw=2
)

best_s = s_vals[best_idx]

best_ess = ess_vals[best_idx]

ax_ess.scatter(
    [best_s],
    [best_ess],
    color="#c67117",
    s=80,
    zorder=5
)

ax_ess.annotate(
    f"Peak ESS ≈ {best_ess:.0f}\nat s = {best_s}",
    (best_s, best_ess),
    textcoords="offset points",
    xytext=(12, -15),
    fontweight="bold",
    color="#c67117",
    fontsize=9.5
)

ax_ess.set_xscale("log")

ax_ess.spines[["top", "right"]].set_visible(False)

ax_ess.set(
    xlabel="Step size (log scale)",
    ylabel="Effective Sample Size (ESS)",
    title="Sampling Efficiency vs Step Size"
)


# Trace plots

for col, idx in enumerate(showcase_indices):

    res = mh_results[idx]

    ax = fig.add_subplot(gs[1, col])

    ax.plot(
        res["chain"][:500],
        color=showcase_colors[col],
        lw=0.9
    )

    ax.spines[["top", "right"]].set_visible(False)

    ax.set(
        xlim=(0, 500),
        ylim=(-4, 4),
        xlabel="Iteration",
        title=(
            f"s = {res['s']} "
            f"(Accept = {res['accept_rate']:.2f}, "
            f"ESS = {res['ess']:.0f})"
        )
    )

    if col == 0:
        ax.set_ylabel("Theta", fontsize=11)

plt.show()
