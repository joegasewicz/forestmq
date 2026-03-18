const std = @import("std");

const log = std.log;

pub const TCP = struct {
    const Self = @This();

    host: []const u8,

    pub fn init(host: []const u8) Self {
        return Self {
            .host = host,
        };
    }

    // pub fn deinit(self: *Self) void {
    //
    // }

    pub fn serve(self: *Self) !void {
        const addr = try std.net.Address.parseIp(self.host, 8752);
        var server = try std.net.Address.listen(addr, .{ .reuse_address = true });
        defer server.deinit();

        log.info("Starting server on {f}", .{addr});
    }
};