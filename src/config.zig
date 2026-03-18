const std = @import("std");

pub const VERSION = "v0.0.0";
const ANSI_GREEN = "\x1b[32m";
const ANSI_RESET = "\x1b[0m";

const preamble =
\\
\\   / ____/ __ \\/ __ \\/ ____/ ___/_  __/  /  |/  / __ \\
\\  / /_  / / / / /_/ / __/  \\__ \\ / /    / /|_/ / / / /
\\ / __/ / /_/ / _, _/ /___ ___/ // /    / /  / / /_/ /
\\/_/    \\____/_/ |_/_____//____//_/    /_/  /_/\\___\\_\\
\\
;

pub fn printPreamble() void {
    std.debug.print("\n{s}{s}{s}\n", .{ANSI_GREEN, preamble, ANSI_RESET});
    std.debug.print("ForestMQ {s}\n", .{VERSION});
    std.debug.print("Serving on fmqp://localhost:8735\n", .{});
}
