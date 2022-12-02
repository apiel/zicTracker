function renderer() {
    CONFIG.sort(function (a, b) {
        return a[1] - b[1];
    });

    render(
        'ADSR envelop:\n  Attack: ' +
            Math.pow(CONFIG[0][2], 2) +
            ' ms\n  Decay: ' +
            CONFIG[1][2] * 10 +
            ' ms\n  Sustain: ' +
            Math.round((CONFIG[2][2] / 127) * 100) +
            '%\n  Release: ' +
            Math.pow(CONFIG[3][2], 2) +
            ' ms'
    );
}

function update(keys) {
    // if (keys.Up) {
    //   count++;
    // } else if (keys.Down) {
    //   count--;
    // }
    // log(JSON.stringify(CONFIG.sort(function (a, b) { return a[1] - b[1]; })));

    // CONFIG.sort(function (a, b) { return b[1] - a[1]; });
    log(JSON.stringify(CONFIG));

    return VIEW_CHANGED;
}
