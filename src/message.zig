
pub const Message = struct {
    topic: []const u8,
    body: []const u8,
    flags: u8 = 0,
};
