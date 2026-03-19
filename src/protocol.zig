const std = @import("std");

const log = std.log;

pub const Protocol = struct {
    const Self = @This();

    header: [6]u8,

    pub fn init(header: [6]u8) Self {
        return Self {
            .header = header,
        };
    }

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