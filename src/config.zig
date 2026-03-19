const std = @import("std");


pub const ANSI_GREEN = "\x1b[32m";
pub const ANSI_RESET = "\x1b[0m";
pub const FMQP_MAGIC = "FMQP";
pub const FMQP_RESP_ERROR = 0x00;
pub const FMQP_RESP_OK = 0x01;
pub const FMQP_VERSION: u16 = 0x0001;

const preamble =
\\
\\   / ____/ __ \\/ __ \\/ ____/ ___/_  __/  /  |/  / __ \\
\\  / /_  / / / / /_/ / __/  \\__ \\ / /    / /|_/ / / / /
\\ / __/ / /_/ / _, _/ /___ ___/ // /    / /  / / /_/ /
\\/_/    \\____/_/ |_/_____//____//_/    /_/  /_/\\___\\_\\
\\
;

pub fn printPreamble(buf: []u8) !void {
    std.debug.print("\n{s}{s}{s}\n", .{ANSI_GREEN, preamble, ANSI_RESET});
    const version = try getFMQVersion(buf);
    std.debug.print("{s}ForestMQ{s} {s}\n", .{ANSI_GREEN, ANSI_RESET, version});
    std.debug.print("Serving on fmqp://localhost:8735\n", .{});
}

pub fn getFMQVersion(buf: []u8) ![]const u8 {
    const high: u8 = @intCast((FMQP_VERSION >> 8) & 0xFF);
    const low: u8 = @intCast(FMQP_VERSION & 0xFF);
    const version = try std.fmt.bufPrint(buf, "v{}.{}", .{high, low});
    return version;
}
