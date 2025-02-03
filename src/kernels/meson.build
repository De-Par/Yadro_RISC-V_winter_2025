kernel_files = files(
    './vec_sum.cl',
)

foreach file : kernel_files 
    configure_file(
        input: file,
        output: '@PLAINNAME@',
        copy: true
    )
endforeach