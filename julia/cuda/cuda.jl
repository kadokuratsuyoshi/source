#=
@30 pkg>add CUDAnative
@30 pkg>add CUDAdrv
@30 pkg>add CuArrays

julia>include cuda.jl
julia> @time @cuda threads=len vadd(d_a, d_b, d_c)
┌ Warning: `haskey(::TargetIterator, name::String)` is deprecated, use `Target(; name=name) !== nothing` instead.
│   caller = llvm_compat(::VersionNumber) at compatibility.jl:176
└ @ CUDAnative ~/.julia/packages/CUDAnative/e0IdN/src/compatibility.jl:176
┌ Warning: `Target(triple::String)` is deprecated, use `Target(; triple=triple)` instead.
│   caller = ip:0x0
└ @ Core :-1
 64.022915 seconds (35.82 M allocations: 1.732 GiB, 3.16% gc time)

julia>c=Base.Array(d_c)
julia>@time @cuda threads=len vadd(d_a, d_b, d_c)
julia>c=Base.Array(d_c)
=#


# listing 16
using CUDAnative, CuArrays

function vadd(a, b, c)
    i = (blockIdx().x-1)*blockDim().x + threadIdx().x
    c[i] = a[i] + b[i]
    return
end

len = 100
a = rand(Float32, len)
b = rand(Float32, len)

d_a = CuArray(a)
d_b = CuArray(b)
d_c = similar(d_a)

@time @cuda threads=len vadd(d_a, d_b, d_c)
