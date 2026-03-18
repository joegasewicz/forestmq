const std = @import("std");
const forestmq_mod = @import("./forestmq.zig");
const tcp_mod = @import("./tcp.zig");

const ForestMQ = forestmq_mod.ForestMQ;
const TCP = tcp_mod.TCP;

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const fmq_allocator = gpa.allocator();

    var forestmq = try ForestMQ.init(fmq_allocator);
    defer forestmq.deinit();

    // Server
    var tcp = TCP.init("127.0.0.1");
    try tcp.serve();
}
