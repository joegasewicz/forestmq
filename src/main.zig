const std = @import("std");
const forestmq_mod = @import("./forestmq.zig");

const ForestMQ = forestmq_mod.ForestMQ;

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const fmq_allocator = gpa.allocator();

    var forestmq = try ForestMQ.init(fmq_allocator);
    defer forestmq.deinit();
}
