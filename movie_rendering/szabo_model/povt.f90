program gerapov
  real,allocatable::x(:),y(:),z(:),angle(:),tamanho(:)
!  real ::xm,ym,t
  real :: comp
  integer :: nr,nc,n
  real:: vx,vy,dreq
  character::cor*20
  real::auxiliar
  character ::a*20,fmtcam*60,fmtesf*60,arquivopov*12
  character :: define*20,leitura_nao_utilizada*20,var*20,ultimo*4
  real::valor
  integer::novo_pov = 0,i
  real::t
  real::look_x, look_y, look_z
  open(10,file="posicoes.dat")
!  open(20,file="obs")
!  read(20,*)n,nv,nc,comp
!  close(20)
  open(20,file="../include/param.h")
  i=0
  do while(i==0)
    read(20,*,END=20,ERR=30)define,var,valor
    !write(*,*)define,var,valor
    goto 40
    30  read(20,*,END=20,ERR=20)define,var,leitura_nao_utilizada
    40  continue
    write(*,*)define,var,valor
    if(var=="Lx")ladox=valor
    if(var=="Ly")ladoy=valor
    if(var=="R_EQ")r=valor
    if(var=="R_ESFERA")look_z=valor
    if(var=="L_CENTRO_X")look_x=valor
    if(var=="L_CENTRO_Y")look_y=valor
  end do
  20 continue
  if(ladox<ladoy)then
    ladoz=ladox*1.8
  else
    ladoz=ladox*0.8
  end if
  write(*,*)"dimensoes do sistema:",ladox,ladoy,ladoz
  call system("rm ray*.pov")
  call system("ls ray*.png > toto")
  open(40,file="toto")
  do while(i==0)
    read(40,*,ERR=50,END=50)var
    ultimo(1:4) = var(4:7)
    ! read(ultimo,*,iostat=stat)  novo_pov
     read(ultimo,*)  novo_pov
  end do
  50 continue
  write(*,*)"ultimo que existe:",novo_pov,var
  !ladox=40;ladoy=80;ladoz=50
!  ladox=comp;ladoy=comp/2.0;ladoz=ladox
  do lup=1,1000
     !do i=1,7
    read(10,*, END=100, ERR=100)t,n

    allocate(x(n+1),y(n+1),z(n+1),angle(n+1),tamanho(n+1))
     !end do
     do i=1,n
       read(10,*,END=100)x(i),y(i),vx,vy,a,dreq
       tamanho(i) = r + dreq
       angle(i)=atan2(vy,vx)
       do while(angle(i)>6.2830)
         angle(i)=angle(i)-6.2830
       end do
       do while(angle(i)<0)
         angle(i)=angle(i)+6.2830
       end do
     end do
     tamanho = tamanho / 2.0
     if(lup>novo_pov)then
       write(*,*)lup,n
       r=0.8
       z=r
       write(arquivopov,'(a3,i4.4,a4)')'ray',lup,'.pov'
       open(30, file = arquivopov)
       fmtcam = '(a16,f8.3,a1,f8.3,a1,f8.3,a9,f8.3,a1,f8.3,a1,f8.3,a2)'
       write(30,*) '#include "colors.inc"'
       write(30,*) '// tempo:',t

       write(30,fmtcam)'camera{location<',look_x,',',10*look_z,',',look_y,'>look_at<',look_x,',',0.0,',',look_y,'>}'
       write(30,'(a14,f8.3,a1,f8.3,a1,f8.3,a13)')'light_source{<',-look_x,',',20*look_z,',',look_y,'>color White}'
       write(30,'(a10,f8.3,a35)') 'plane { y,', 0., ' pigment{color rgb<0.0, 0.0, 0.0>}}'
       fmtesf = '(a8,f12.7,a1,f5.2,a1,f12.7,a2,f8.3,a23,a13,a30)'
       write(30,*)"disc{<",look_x,",",r,",",look_y,">, <0.0, 1.0, 0.0>, ",look_z,", pigment{ color rgb<0.5, 0.5 ,0.5>}}"
       
       do i = 1, n
         auxiliar=360*(angle(i))/(6.2830)
         write(cor,'(a1,i3,a9)')'<',int(auxiliar),',1.0,0.5>'
         write(30,fmtesf)'sphere{<',x(i),',',z(i),',',y(i),'>,',tamanho(i),'pigment{color CHSL2RGB(',cor,')}finish{phong 1}}'
       end do
       close(30)
     end if
     deallocate(x,y,z,angle,tamanho)
   end do
100 continue   
   call system("rm toto")
end program gerapov
