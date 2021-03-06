`default_nettype none

module read_buffer(
    input reset_n,
    input clk,

    input clear,

    input [31:0] mem_read_data,
    input mem_read_data_valid,

    output logic [31:0] data[0:1],
    output logic [1:0] count);

    logic [1:0] count_next;

    always_comb begin
        count_next = count;

        if (clear) begin
            count_next = 2'h0;
        end

        if (mem_read_data_valid) begin
            count_next = count + 2'h1;
        end
    end

    always_ff @(posedge clk) begin
        if (!reset_n) begin
            data <= '{default:0};
            count <= 2'h0;
        end
        else begin
            if (mem_read_data_valid) begin
                data[count[0]] <= mem_read_data;
            end

            count <= count_next;
        end
    end

endmodule
