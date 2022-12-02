function renderer() {
    useColor(1, 2, COLOR_LIGHT);
    useColor(1, 3, COLOR_DARK, 6);

    useColor(2, 2, COLOR_LIGHT);
    useColor(2, 3, COLOR_DARK, 5);

    useColor(3, 2, COLOR_LIGHT);
    useColor(3, 3, COLOR_DARK, 7);

    useColor(4, 2, COLOR_LIGHT);
    useColor(4, 3, COLOR_DARK, 7);

    CONFIG.sort(function (a, b) {
        return a[1] - b[1];
    });

    render(
        'Envelop:\n  Attack:  ' +
            Math.pow(CONFIG[0][2], 2) +
            ' ms\n  Decay:   ' +
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
    // log(JSON.stringify(CONFIG));

    return VIEW_CHANGED;
}
