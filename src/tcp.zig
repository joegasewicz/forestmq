const std = @import("std");

const log = std.log;
const Connection = std.net.Server.Connection;

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

        while (true) {
            const conn = server.accept() catch |err| {
                log.err("failed tp accept connection: {s}", .{@errorName(err)});
                continue;
            };
            const thread = std.Thread.spawn(.{}, TCP.accept, .{conn}) catch |err| {
                log.err("unable to spawn connection thread: {s}", .{@errorName(err)});
                conn.stream.close();
                continue;
            };
            thread.detach();
        }
    }

    pub fn accept(conn: Connection) !void {
        defer conn.stream.close();
    }
};