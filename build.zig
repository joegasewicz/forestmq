const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});


    const exe = b.addExecutable(.{
        .name = "main",
        .root_module = b.createModule(.{
            .root_source_file = b.path("src/main.zig"),
            .target = target,
            .optimize = optimize,
            // .imports = &.{
            //
            // },
        }),
    });

    // ----------------------------------
    // tests
    const exe_tests = b.addTest(.{
        .root_module = exe.root_module,
    });
    const run_exe_tests = b.addRunArtifact(exe_tests);
    const test_step = b.step("test", "Run tests");
    test_step.dependOn(&run_exe_tests.step);

    // ==================================
    // forestmq Tests
    // ==================================
    const forestmq_module = b.addModule("forestmq", .{
        .root_source_file = b.path("src/forestmq.zig"),
        .target = target,
        .optimize = optimize,
    });
    const forestmq_tests = b.addTest(.{
        .root_module = forestmq_module
    });
    const run_forestmq_test = b.addRunArtifact(forestmq_tests);
    test_step.dependOn(&run_forestmq_test.step);
    // ------------------------------------
    // ==================================
    // forestmq Tests
    // ==================================
    const queue_module = b.addModule("queue", .{
        .root_source_file = b.path("src/queue.zig"),
        .target = target,
        .optimize = optimize,
    });
    const queue_tests = b.addTest(.{
        .root_module = queue_module,
    });
    const run_queue_test = b.addRunArtifact(queue_tests);
    test_step.dependOn(&run_queue_test.step);
    // ------------------------------------
    // ==================================
    // topics Tests
    // ==================================
    const topic_module = b.addModule("topic", .{
        .root_source_file = b.path("src/topic.zig"),
        .target =  target,
        .optimize = optimize,
    });
    const topic_tests = b.addTest(.{
        .root_module = topic_module,
    });
    const run_topic_test = b.addRunArtifact(topic_tests);
    test_step.dependOn(&run_topic_test.step);
    // Run steps
    b.installArtifact(exe);
    const run_step = b.step("run", "Run the app");
    const run_cmd = b.addRunArtifact(exe);
    run_step.dependOn(&run_cmd.step);
    run_cmd.step.dependOn(b.getInstallStep());

    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

}