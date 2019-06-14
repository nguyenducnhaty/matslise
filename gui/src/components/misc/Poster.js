import React, { Component } from "react";
import loadMatslise from "../../lib/loadMatslise";
import * as THREE from "three";
import OrbitControls from "orbit-controls-es6";

class Poster extends Component {
  state = {
    loaded: false,
    x: [-5.5, 5.5],
    y: [-5.5, 5.5],
    V: (x, y) => (1 + x * x) * (1 + y * y),
    se2d: null
  };

  SE2D = null;

  componentDidMount() {
    loadMatslise.then(({ Matslise, SE2D }) => {
      this.SE2D = SE2D;

      this.setState({
        loaded: true,
        se2d: new SE2D(this.state.V, ...this.state.x, ...this.state.y, {
          sectorCount: 31,
          stepsPerSector: 3,
          nested: {
            sectorCount: 31
          }
        })
      });
    });
  }

  render() {
    window.se2d = this.state.se2d;

    if (!this.state.loaded) return <div />;
    return (
      <div>
        <div ref={e => this.injectThree(e)} />
      </div>
    );
  }

  computeEigenfunctions(E, x, y) {
    return this.state.se2d.computeEigenfunction(E, x, y);
  }

  calculateGeometry(x, y, z) {
    const get = (r, s, i) => (s === 0 ? r[i] : (1 - s) * r[i] + s * r[i + 1]);

    return new THREE.ParametricGeometry(
      (u, v, p) => {
        const vx = u * (x.length - 1),
          vy = v * (y.length - 1);
        const ix = Math.floor(vx),
          iy = Math.floor(vy);
        const sx = vx - ix,
          sy = vy - iy;
        p.x = get(x, sx, ix);

        if (sx === 0) p.y = get(z[ix], sy, iy);
        else p.y = (1 - sx) * get(z[ix], sy, iy) + sx * get(z[ix + 1], sy, iy);

        p.z = get(y, sy, iy);
      },
      x.length,
      y.length
    );
  }

  buildGraph(x, y, z) {
    const scale = 3;
    z = z.map(r => r.map(v => v * scale));

    const group = new THREE.Group();
    const materialSettings = {
      color: 0x1e64c8,
      side: THREE.DoubleSide,
      flatShading: true
    };
    let splits = [];
    for (let i = 0; i <= 17; ++i) splits.push(i / 17);

    splits = splits.map(a => Math.round(a * y.length));

    splits.forEach((s, i) => {
      if (i + 1 === splits.length) return;
      const moreSettings =
        i % 2
          ? {}
          : {
              opacity: 1,
              color: 0x2d8ca8
            };
      group.add(
        new THREE.Mesh(
          this.calculateGeometry(
            x,
            y.slice(s, splits[i + 1] + 1),
            z.map(v => v.slice(s, splits[i + 1] + 1))
          ),
          new THREE.MeshStandardMaterial({
            ...materialSettings,
            ...moreSettings
          })
        )
      );
    });

    return group;
  }

  buildObjects() {
    const group = new THREE.Group();

    const xn = 300,
      yn = 300;
    const {
      x: [xmin, xmax],
      y: [ymin, ymax]
    } = { x: [-4.5, 4.5], y: [-4.5, 4.5] };

    const x = [];
    for (let i = 0; i <= xn; ++i) x.push(xmin + ((xmax - xmin) * i) / xn);
    const y = [];
    for (let i = 0; i <= yn; ++i) y.push(ymin + ((ymax - ymin) * i) / yn);

    [19.594].forEach(approxE => {
      const E = this.state.se2d.findEigenvalue(approxE);
      console.log(E);
      this.computeEigenfunctions(E, x, y).forEach(z => {
        group.add(this.buildGraph(x, y, z));
      });
    });

    return group;
  }

  buildScene() {
    this.camera = new THREE.PerspectiveCamera(
      70,
      window.innerWidth / window.innerHeight,
      0.01,
      1000
    );
    this.camera.position.x = 10;
    this.camera.position.y = 10;
    this.camera.position.z = 10;

    this.scene = new THREE.Scene();
    this.scene.background = new THREE.Color(0xffffff);

    this.scene.add(this.buildObjects());

    this.scene.add(new THREE.AmbientLight(0x404040,3));
    [[0, 20, 20], [0, -50, 50]].forEach(([x, y, z]) => {
      const light = new THREE.PointLight(0x808080, 1, 0);
      light.position.x = x;
      light.position.y = y;
      light.position.z = z;
      this.scene.add(light);
    });

    this.renderer = new THREE.WebGLRenderer({ antialias: true });
    this.renderer.setSize(window.innerWidth, window.innerHeight);

    const controls = new OrbitControls(this.camera, this.renderer.domElement);
    controls.enabled = true;
    controls.maxDistance = 1500;
    controls.minDistance = 0;
  }

  animate() {
    requestAnimationFrame(() => this.animate());

    this.renderer.render(this.scene, this.camera);
  }

  injectThree(element) {
    if (!element) return;
    if (!this.renderer) {
      this.buildScene();
      this.animate();
    }
    element.appendChild(this.renderer.domElement);
  }
}

export default Poster;
