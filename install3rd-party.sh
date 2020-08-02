install_spdlog()
{
    git clone https://github.com/gabime/spdlog.git
    cd spdlog && mkdir build && cd build
    cmake .. && make -j
}

case $1 in
    *) install_spdlog ;;
esac
