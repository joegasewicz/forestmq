const std = @import("std");

const log = std.log;

/// Struct reflects all FMQP protocol operations
///
/// Header data frame:
/// | PROTOCOL (4 bytes)     |
/// | VERSION (2 bytes)      |
/// | FLAGS (1 byte)         |
/// | TOPIC LENGTH (2 bytes) |
/// | BODY LENGTH (2 bytes)  |
///
/// Body data frame:
/// | TOPIC                  |
/// | BODY                   |
pub const Protocol = struct {

    const Self = @This();

    header: [6]u8,

    pub fn init(header: [6]u8) Self {
        return Self {
            .header = header,
        };
    }

    /// Gets the protocol bytes e.g. `FMQP`
    pub fn protocol(self: *Self) []const u8 {
        return self.header[0..4];
    }

    pub fn versionBytes(self: *Self) []const u8 {
        return self.header[4..6];
    }

    pub fn getVersion(buf: []u8, bytes: []const u8) ![]const u8 {
        const version = try std.fmt.bufPrint(buf, "v{}.{}", .{bytes[0], bytes[1]});
        return version;
    }

    pub fn printClientHeader(self: *Self, buf: []u8) !void {
        const version_str = try Protocol.getVersion(buf, self.versionBytes());
        std.debug.print(
            \\Header received from client:
            \\- protocol: {s}
            \\- version: {s}
        , .{ self.protocol(), version_str });
    }
};